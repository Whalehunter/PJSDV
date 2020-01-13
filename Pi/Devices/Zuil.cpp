#include "Zuil.hpp"

using json = nlohmann::json;

Zuil::Zuil(int n, Appartement* ap): Device(n, ap), nood(0), brand(0), zoemer(0), timer(0)
{
    std::cout << "Zuil aangemaakt" << std::endl;
}

Zuil::~Zuil()
{
}

void Zuil::operator()()
{
    char buffer[256];

    while(1) {
        /* get and store JSON values */

        sendMsg("getStatus");

        memset(buffer, 0, sizeof(buffer));
        if(recv(sock, buffer, 255, 0) < 1) {
            std::cout << "Zuil disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

      //  std::cout << buffer << std::endl;

        try {
            auto j_zuil = json::parse(buffer);

            knopValue = j_zuil.at("knopValue");
            sensorValue = j_zuil.at("sensorValue");
        }
        catch(json::exception& e) {
            std::cout << "Exception error at Zuil: " << e.what() << std::endl;
        }

        if(knopValue) {
            noodAlarmAan();
        }
        if(sensorValue >= 920) {
            brandAlarmAan();
        }

        /*if(timer != 0 && zoemer == 1 && (((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 1.5) && nood == 0) {
            deurBelUit();
        }*/
    }

    close(sock);
}

void Zuil::noodAlarmAan()
{
    zoemerAan();

    nood = 1;
}

void Zuil::noodAlarmUit()
{
    if (!brand && !timer)
        zoemerUit();
    nood = 0;
}

void Zuil::brandAlarmAan()
{
    if (!nood && !timer)
        zoemerAan();
    brand = 1;
}

void Zuil::brandAlarmUit()
{
    if (!nood && !timer)
        zoemerUit();
    brand = 0;
}

void Zuil::deurBelAan()
{
    if (!nood && !brand)
        zoemerAan();
    timer = clock();
}

void Zuil::deurBelUit()
{
    if (!nood && !brand)
        zoemerUit();
    timer = 0;
}

void Zuil::zoemerAan() {
    sendMsg("zoemerAan");
    zoemer = 1;
}

void Zuil::zoemerUit() {
    sendMsg("zoemerUit");
    zoemer = 0;
}

json Zuil::getStatus()
{
    json zuilData;
    zuilData["Zuil"] = {{"Zoemer", zoemer}, {"Noodalarm", nood}, {"Brandalarm", brand}, {"Gasmeter", sensorValue}, {"Knop", knopValue}};

    return zuilData;
}
