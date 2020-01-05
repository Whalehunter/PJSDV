#include "Muur.hpp"

Muur::Muur(int n, Appartement* ap): Device(n, ap)
{
	std::cout << "Muur aangemaakt" << std::endl;
}

Muur::~Muur()
{
}

void Muur::operator()()
{
    char buffer[256];

    strcpy(buffer, "getStatus\r");

    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::stringstream s(buffer);
        s >> knopValue;

        std::cout << "Muur: " << knopValue << std::endl;

        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

int Muur::getStatus()
{
    return knopValue;
}

void Muur::updateWaardes()
{
	//ontvang waardes van de WEMOS en sla die op in eigen variabelen
}

void Muur::RGBsterkte(potmeter p)
{
	//stuur RGB lichtsterkte naar de WEMOS
}

void Muur::LCDdimmen()
{
	//LCD display aan zetten op de WEMOS
}

void Muur::LCDdoorlaten()
{
	//LCD display uit zetten op de WEMOS
}

void Muur::ledBinnenAan()
{
	//binnenled van Deur aan doen
}

void Muur::ledBinnenUit()
{
	//binnenled van Deur uit doen
}