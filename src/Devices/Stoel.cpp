//scp ProjectSD pi@192.168.2.90:/home/pi/ProjectSD/Kaas

#include "Stoel.hpp"


using json = nlohmann::json;


Stoel::Stoel(int n, Appartement* ap): Device(n, ap)
{
    ledStatus = 0;
    std::cout << "Stoel aangemaakt" << std::endl;
}

Stoel::~Stoel()
{
}

void Stoel::operator()()//Overloaded functies moeten met 2 haakjes zodat je er zoveel mogelijk over kan sturen als je wilt.
{
    char buffer[256];
    int drukSensorPrev = 0;
    int drukknopPrev = 0;

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
            auto j_deur = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

            drukknop = j_deur.at("drukknop");
            drukSensor = j_deur.at("drukSensor");
        }
        catch(json::parse_error) {
            std::cout << "parse error" << std::endl;
        }
        std::cout << drukknop << std::endl;

        if ((drukknop == 1) && (drukknopPrev != drukknop) && (ledStatus == 0)){
        	ledAan();
        	ledStatus = 1;
        }
        else if ((drukknop == 1) && (drukknopPrev != drukknop) && (ledStatus == 1)){
            ledUit();
            ledStatus = 0;
        }

        if ((drukknop == 1) && (drukSensor == 1) && (trilStatus == 0) && ((drukknopPrev != drukknop) || (drukSensorPrev != drukSensor))){
            trilAan();
            trilStatus = 1;
        }
        else if ((drukknop == 1) && (drukSensor == 1) && (trilStatus == 1) && ((drukknopPrev != drukknop) || (drukSensorPrev != drukSensor))){
            trilUit();
            trilStatus = 0;
        }
        drukSensorPrev = drukSensor;
        drukknopPrev = drukknop;
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Stoel::getStatus()
{
    return knopValue;
}

void Stoel::ledAan(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "ledAan\r");
	sendMsg(buffer);
}

void Stoel::ledUit(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "ledUit\r");
	sendMsg(buffer);
}

void Stoel::trilAan(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "trilAan\r");
	sendMsg(buffer);
}

void Stoel::trilUit(){
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "trilUit\r");
	sendMsg(buffer);
}
