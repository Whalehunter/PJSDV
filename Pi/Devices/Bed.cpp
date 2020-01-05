#include "Bed.hpp"

Bed::Bed(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{
}

void Bed::operator()()
{
    char buffer[256];

    strcpy(buffer, "getStatus\r");

    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::stringstream s(buffer);
        s >> knopValue;

        std::cout << "Bed: " << knopValue << std::endl;

        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

int Bed::getStatus()
{
    return knopValue;
}

void Bed::updateWaardes()
{

}

void activeerBewegingssensor()
{
	
}

void deactiveerBewegingssensor()
{
	
}

void toggleLed()
{
	if(ledStatus == 0 & drukknop == 0){
		//led blijft uit
	}
	else if (ledStatus == 0 & drukknop == 1){
		//led gaat aan
	}
	else if (ledStatus == 1 & drukknop == 0){
		//led blijft aan
	}
	else if (ledStatus == 1 & drukknop == 1){
		//led gaat uit
	}
}