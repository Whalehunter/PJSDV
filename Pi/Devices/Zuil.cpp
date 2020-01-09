#include "Zuil.hpp"

using json = nlohmann::json;

Zuil::Zuil(int n, Appartement* ap): Device(n, ap), nood(0), brand(0), zoemer(0)
{
    std::cout << "Zuil aangemaakt" << std::endl;
}

Zuil::~Zuil()
{
}

void Zuil::operator()()
{
    char buffer[256];
    int knopValuePrev = 0;

    while(1) {
        /* get and store JSON values */

        sendMsg("getStatus\r");

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

        if((knopValue == 1 && knopValuePrev != knopValue) || sensorValue >= 915) {
            noodAlarmAan();

            nood = knopValue;
            brand = (sensorValue >= 915);
        }
        if(sensorValue == 920) {
            brandAlarm(1);
        }

        if(zoemer == 1 && (((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 1.5) && nood == 0) {
            deurBelUit();
        }

        knopValuePrev = knopValue;
    }

    close(sock);
}

void Zuil::noodAlarmAan()
{
    sendMsg("noodAlarm\r");

    nood = 1;
    zoemer = 1;
}

void Zuil::noodAlarmUit()
{
    sendMsg("noodAlarmUit\r");

    zoemer = 0;
    nood = 0;
}

void Zuil::brandAlarm(int n)
{
    sendMsg("brandAlarm\r");

    brand = n;
}

void Zuil::deurBelAan()
{
    sendMsg("deurBelAan\r");

    zoemer = 1;
    timer = clock();
}

void Zuil::deurBelUit()
{
    sendMsg("deurBelUit\r");

    zoemer = 0;
}

json Zuil::getStatus()
{
    json zuilData;
    zuilData["Zuil"] = {{"Zoemer", zoemer}, {"Noodalarm", nood}, {"Brandalarm", brand}, {"Gasmeter", sensorValue}, {"Knop", knopValue}};

    return zuilData;
}
