/*
 * Koelkast.cpp
 *
 *  Created on: Jan 8, 2020
 *      Author: programmer
 */

#include "Koelkast.hpp"

using json = nlohmann::json;

Koelkast::Koelkast(int n, Appartement* ap): Device(n, ap){
    std::cout << "Koelkast aangemaakt" << std::endl;
}

Koelkast::~Koelkast(){

}

void Koelkast::operator()(){
    char buffer[256];

    while(1) {
		/* get and store JSON values */

		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, "getStatus\r");
		sendMsg(buffer);

		memset(buffer, 0, sizeof(buffer));
		if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
			std::cout << "Deur disconnected from socket: " << sock << std::endl;
			close(sock);
			return;
		}

		std::cout << buffer << std::endl;

		try {
			auto j_koelkast = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

			koelkastDeur = j_koelkast.at("deur");
			NTC1 = j_koelkast.at("NTC1");
			NTC2 = j_koelkast.at("NTC2");
		}
		catch(json::parse_error) {
			std::cout << "parse error" << std::endl;
		}

		if ((koelkastDeur == 0) && !((std::clock() - timer / (double) CLOCKS_PER_SEC) >= 10.0)){
			timer = std::clock();
			peltierUit();
			fanUit();
		}
		else {
			peltierAan();
			fanAan();
		}
		if ((koelkastDeur == 0) && ((std::clock() - timer / (double) CLOCKS_PER_SEC) >= 10.0)) {
			koelAlarm = 1;
			fanAan();
		}
	}
	close(sock);
	std::cout << "Connection closed on socket " << sock << std::endl;
}

json Koelkast::getStatus(){
    return knopValue;
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
}

void Koelkast::peltierUit(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "peltierUit\r");
	sendMsg(buffer);
}
