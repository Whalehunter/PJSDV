//scp Kaas pi@192.168.2.90:/home/pi/Kaas

#include "Stoel.hpp"


using json = nlohmann::json;


Stoel::Stoel(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Stoel aangemaakt" << std::endl;
}

Stoel::~Stoel()
{
}

void Stoel::operator()()//Overloaded functies moeten met 2 haakjes zodat je er zoveel mogelijk over kan sturen als je wilt.
{
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

		auto j_deur = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

		drukknop = j_deur.at("drukknop");
		trilStatus = j_deur.at("trilStatus");

		if ((drukknop == 1) & (ledStatus == 0)){
			sendMsg("ledAan/r");
			ledStatus = 1;
		}/*
		else if ((drukknop == 1) & (ledStatus == 1)){
			sendMsg("ledUit/r");
			ledStatus = 0;
		}*/

		if ((drukknop == 1) & (druksensor == 1) & (trilStatus == 0)){
				sendMsg("trilAan/r");
				trilStatus = 1;
		}/*
		else if ((drukknop == 1) & (druksensor == 1) & (trilStatus == 1)){
			sendMsg("trilUit/r");
			trilStatus = 0;
		}*/
		/*ledAanUit();
		trilAanUit();*/
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Stoel::getStatus()
{
    return knopValue;
}

void Stoel::ledAanUit(){
	if ((drukknop == 1) & (ledStatus == 0)){
		sendMsg("ledAan/r");
		ledStatus = 1;
	}
	else if ((drukknop == 1) & (ledStatus == 1)){
		sendMsg("ledUit/r");
		ledStatus = 0;
	}
}

void Stoel::trilAanUit(){
	if ((drukknop == 1) & (druksensor == 1) & (trilStatus == 0)){
		sendMsg("trilAan/r");
		trilStatus = 1;
	}
	else if ((drukknop == 1) & (druksensor == 1) & (trilStatus == 1)){
		sendMsg("trilUit/r");
		trilStatus = 0;
	}
}

