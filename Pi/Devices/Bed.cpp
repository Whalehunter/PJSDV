#include "Bed.hpp"

using json = nlohmann::json;

Bed::Bed(int n, Appartement* ap): Device(n, ap), state(UIT), druksensor(0), knop(0)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{}

void Bed::operator()()
{
    int knopPrev = 0;

    while(1){
        /*Get and store JSON values, break from while Bed is gone*/
        if(!updateStatus()) break;

        /*State Machine*/
        switch(state){
            case AAN:
                if (knop && !knopPrev){
                    ToggleLed(0);
                }
                break;
            case UIT:
                if (knop && !knopPrev){
                    ToggleLed(1);
                }
        }
        knopPrev = knop;
    }
}

nlohmann::json Bed::getStatus()
{
    json bedData;
    bedData["Bed"] = {{"Bed", state ? "aan" : "uit"}, {"knop", knop}, {"drukSensor", druksensor}};
    return bedData;
}

bool Bed::updateStatus()
{
    char buffer[256] = {0};
    sendMsg("getStatus\r");
    if(recv(sock, buffer, 255, 0) < 1){
        std::cout << "Bed disconnected from socket: " << sock << std::endl;
        close(sock);
        return false;
    }

    try {
        auto j_bed = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

        knop = j_bed.at("knop");
        druksensor = j_bed.at("druksensor");
    }
    catch(json::exception& e) {
        std::cout << "Parsing error at Bed on socket " << sock << std::endl;
    }
    return true;
}

void Bed::ToggleLed(int i){
    if (i) {
        ledAan();
    } else {
        ledUit();
    }
}

void Bed::ledAan() {
    sendMsg("lampAan\r");
    state = AAN;
}

void Bed::ledUit() {
    sendMsg("lampUit\r");
    state = UIT;
}

int Bed::getDruksensor(){
    return druksensor;
}
