#include "Stoel.hpp"

using json = nlohmann::json;

Stoel::Stoel(int n, Appartement* ap): Device(n, ap)//Stoel Aanmaken
{
    std::cout << "Stoel aangemaakt" << std::endl;
}

Stoel::~Stoel()//Stoel vernietigen
{}

//Overloaded functies moeten met 2 haakjes zodat je er zoveel als je wilt over kan sturen.
void Stoel::operator()()//Opereratie functie van stoel
{
    char buffer[256] = {0};
    int drukknopPrev = 0;

    while(1) {
        sendMsg("getStatus\r");//Stuur getStatus naar device

        if(recv(sock, buffer, 255, 0) < 1) {
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

        if (!drukSensor && trilStatus){
            trilUit();
        } else if (drukknop && !drukknopPrev && drukSensor){
            toggleTril();
        }

        drukknopPrev = drukknop;
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Stoel::getStatus()//Get Status voor de GUI
{
    json stoel;
    stoel["Stoel"] = {{"Knop", drukknop}, {"Lamp", ledStatus}, {"Massage", trilStatus}, {"Drukplaat", drukSensor}};
    return stoel;
}

void Stoel::toggleLed() {//Leeslamp toggle
    if (ledStatus) {
        Stoel::ledUit();
    } else {
        Stoel::ledAan();
    }
}

void Stoel::ledAan(){//Leeslamp Aan
    sendMsg("ledAan\r");
    ledStatus = 1;
}

void Stoel::ledUit(){//Leeslamp Uit
    sendMsg("ledUit\r");
    ledStatus = 0;
}

void Stoel::toggleTril() {//Trill element toggle
    if (trilStatus) {
        Stoel::trilUit();
    } else {
        Stoel::trilAan();
    }
}

void Stoel::trilAan(){//Trill element Aan
    sendMsg("trilAan\r");
    trilStatus = 1;
}

void Stoel::trilUit(){//Trill element Uit
    sendMsg("trilUit\r");
    trilStatus = 0;
}
