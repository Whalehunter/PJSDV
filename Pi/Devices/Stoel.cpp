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
    char buffer[256] = {0};
    int drukknopPrev = 0;

    while(1) {
        /* get and store Stoel status */
        sendMsg("getStatus\r");

        if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
            std::cout << "Stoel disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        try {
            auto j_stoel = json::parse(buffer);

            drukknop = j_stoel.at("drukknop");
            drukSensor = j_stoel.at("drukSensor");
        }
        catch(json::exception& e) {
            std::cout << e.what() << std::endl;
        }

        if (drukknop && !drukknopPrev){
            toggleLed();
        }

        if (!drukSensor){
            trilUit();
        } else if (drukknop && !drukknopPrev){
            toggleTril();
        }

        drukknopPrev = drukknop;
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Stoel::getStatus()
{
    json stoel;
    stoel["Stoel"] = {{"Knop", drukknop}, {"Lamp", ledStatus}, {"Massage", trilStatus}, {"Drukplaat", drukSensor}};
    return stoel;
}

void Stoel::toggleLed() {
    if (ledStatus) {
        Stoel::ledUit();
    } else {
        Stoel::ledAan();
    }
}

void Stoel::ledAan(){
    sendMsg("ledAan\r");
    ledStatus = 1;
}

void Stoel::ledUit(){
    sendMsg("ledUit\r");
    ledStatus = 0;
}

void Stoel::toggleTril() {
    if (trilStatus) {
        Stoel::trilUit();
    } else {
        Stoel::trilAan();
    }
}

void Stoel::trilAan(){
    sendMsg("trilAan\r");
    trilStatus = 1;
}

void Stoel::trilUit(){
    sendMsg("trilUit\r");
    trilStatus = 0;
}
