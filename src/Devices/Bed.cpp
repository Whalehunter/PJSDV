#include "Bed.hpp"

using json = nlohmann::json;

Bed::Bed(int n, Appartement* ap): Device(n, ap), state(UIT), druksensor(0), knop(0)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{
}

void Bed::operator()()
{
    char buffer[256];
    int knopPrev = 0;

    while(1){
    	memset(buffer, 0 , sizeof(buffer));
    	strcpy(buffer, "getStatus\r");
    	sendMsg(buffer);

    	memset(buffer, 0, sizeof(buffer));
    	if(recv(sock, buffer, 255, 0) < 1){
    		std::cout << "Bed disconnected from socket: " << sock << std::endl;
    		close(sock);
    		return;
    	}

        try {
            auto j_bed = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

            knop = j_bed.at("knop");
            druksensor = j_bed.at("druksensor");
        }
        catch(json::parse_error) {
            std::cout << "Parsing error at Bed on socket " << sock << std::endl;
        }
        switch(state){
            case AAN:
                if (knop == 1 && knopPrev != knop){
                    ToggleLed(1);
                }
                break;
            case UIT:
                if (knop == 1 && knopPrev != knop){
                    ToggleLed(0);
                }
        }
        knopPrev = knop;
    }
}

nlohmann::json Bed::getStatus()
{
    json bedData = {{"Bed", state ? "aan" : "uit"}, {"knop", knop}, {"drukSensor", druksensor}};

    return bedData;

}

void Bed::ToggleLed(int i){
    if(i==1){
        char buff[256];
        memset(buff, 0, sizeof(buff));
        strcpy(buff, "lampAan\r");
        sendMsg(buff);
        state = AAN;
    }else if (i == 0){
        char buff[256];
        memset(buff, 0, sizeof(buff));
        strcpy(buff, "lampUit\r");
        sendMsg(buff);
        state = UIT;
    }
}
