/*
 * Koelkast.cpp
 *
 *  Created on: Jan 8, 2020
 *      Author: programmer
 */

#include "Koelkast.hpp"

using json = nlohmann::json;

Koelkast::Koelkast(int n, Appartement* ap): Device(n, ap), openTimer(0){
    std::cout << "Koelkast aangemaakt" << std::endl;
}

Koelkast::~Koelkast()
{}

void Koelkast::operator()(){
    while(1) {
		/* get and store JSON values */

        if (!updateStatus()) break;

		if ((koelkastDeur == 0) && (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) <= 5.0)){
			peltierUit();/*
			if (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) == 0.0){
				openTimer = std::clock();

			}*/
		}
		else if ((koelkastDeur == 0) && (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) >= 5.0)){
			koelAlarm = 1;
			fanUit();
			peltierUit();
		}
		else{
			openTimer = std::clock();
			peltierAan();
			fanAan();

		}
	//	std::cout << openTimer << std::endl;
	}
	close(sock);
	std::cout << "Connection closed on socket " << sock << std::endl;
}

json Koelkast::getStatus(){
    return knopValue;
}

bool Koelkast::updateStatus(){
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, "getStatus\r");
    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));
    if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
        std::cout << "Koelkast disconnected from socket: " << sock << std::endl;
        close(sock);
        return false;
    }

    try {
        auto j_koelkast = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

        koelkastDeur = j_koelkast.at("deur");
        NTC1 = j_koelkast.at("NTC1");
        NTC2 = j_koelkast.at("NTC2");
    }
    catch(json::exception& e) {
        std::cout << "parse error" << std::endl;
    }
    return true;
}

void Koelkast::disableKoelAlarm(){
	koelAlarm = 0;
}

void Koelkast::fanAan(){
	sendMsg("fanAan\r");
}

void Koelkast::fanUit(){
	sendMsg("fanUit\r");
}

void Koelkast::peltierAan(){
    sendMsg("peltierAan\r");
}

void Koelkast::peltierUit(){
	sendMsg("peltierUit\r");
}
