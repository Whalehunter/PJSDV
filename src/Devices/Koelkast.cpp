/*
 * Koelkast.cpp
 *
 *  Created on: Jan 8, 2020
 *      Author: programmer
 */

#include "Koelkast.hpp"
/*
Koelkast::Koelkast(int n, Appartement* ap): Device(n, ap){
    std::cout << "Koelkast aangemaakt" << std::endl;
}

Koelkast::~Koelkast(){

}

void Koelkast::operator()(){
	char buffer[256];

	strcpy(buffer, "getStatus\r");

	sendMsg(buffer);

	memset(buffer, 0, sizeof(buffer));

	while(recvMsg(buffer)) {
		std::stringstream s(buffer);
		s >> knopValue;

		std::cout << "Koelkast: " << knopValue << std::endl;

		strcpy(buffer, "getStatus\r");
		sendMsg(buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	close(sock);
	std::cout << "Connection closed on socket " << sock << std::endl;
}

int Koelkast::getStatus(){
	return knopValue;
}

void Koelkast::timeOut(){
	if (deur == 0 & timerAanUit == 0){
		//start timer
		timerAanUit = 1;
	}
	if (deur == 1 & timerAanUit == 1){
		//stop timer
		timerAanUit = 0;
	}
	if (deur == 0 & timer expired){
		//Alarm!
	}
}*/
