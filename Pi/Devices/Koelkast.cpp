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
    char buffer[256];

    while(1) {
		/* get and store JSON values */

		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "getStatus\r");
		sendMsg(buffer);

		memset(buffer, 0, sizeof(buffer));
		if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
			std::cout << "Koelkast disconnected from socket: " << sock << std::endl;
			close(sock);
			return;
		}

		try {
			auto j_koelkast = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

			koelkastDeur = j_koelkast.at("deur");
			NTC1 = j_koelkast.at("NTC1");
			NTC2 = j_koelkast.at("NTC2");
		}
		catch(json::parse_error) {
			std::cout << "parse error" << std::endl;
		}

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
    json koelk;
    koelk["Koelkast"] = {{"Deur", koelkastDeur}, {"Koelelement", }, {"m1", NTC1}, {"m2", NTC2}};
    return koelk;
}

void Koelkast::disableKoelAlarm(){
	koelAlarm = 0;
}

void Koelkast::fanAan(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "fanAan\r");
	sendMsg(buffer);
}

void Koelkast::fanUit(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "fanUit\r");
	sendMsg(buffer);
}

void Koelkast::peltierAan(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "peltierAan\r");
	sendMsg(buffer);

        koelelement = 1;
}

void Koelkast::peltierUit(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "peltierUit\r");
	sendMsg(buffer);

        koelelement = 0;
}
