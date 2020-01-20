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
			peltierUit();
		}
		else if ((koelkastDeur == 0) && (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) >= 5.0)){
			koelAlarm = 1;
			fanUit();
			peltierUit();
		}
		else {
			openTimer = std::clock();
			peltierAan();
			fanAan();
		}
		tempOut = calculateCelsius(NTC1);
		tempIn = calculateCelsius(NTC2);
	}
	close(sock);
	std::cout << "Connection closed on socket " << sock << std::endl;
}

json Koelkast::getStatus(){
	json koelk;
	koelk["Koelkast"] = {{"Deur", koelkastDeur}, {"Koelelement", koelelement}, {"m1", tempOut}, {"m2", tempIn}, {"Fan", fan}, {"Alarm", koelAlarm}};
	return koelk;
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
	koelelement = 1;
}

void Koelkast::peltierUit(){
	sendMsg("peltierUit\r");
	koelelement = 0;
}

float Koelkast::calculateCelsius(float i){
	//Formule gevonden op: https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html
	float C = 1.00 / (1.00 / 298.15 + 1.00 / 3380.00*(log (1023.00 / i - 1.00))) - 273.15;
	return C;
}
