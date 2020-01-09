#include "Zuil.hpp"

using json = nlohmann::json;

Zuil::Zuil(int n, Appartement* ap): Device(n, ap), nood(0), brand(0)
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

        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);

        memset(buffer, 0, sizeof(buffer));
        if(recv(sock, buffer, 255, 0) < 1) {
            std::cout << "Zuil disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        try {
            auto j_zuil = json::parse(buffer);

            knopValue = j_zuil.at("knopValue");
            sensorValue = j_zuil.at("sensorValue");
        }
        catch(json::exception& e) {
            std::cout << "Exception error at Zuil: " << e.what() << std::endl;
        }

        if(knopValue == 1 && knopValuePrev != knopValue) {
            noodAlarm(1);
        }
        if(sensorValue == 1) {
            brandAlarm(1);
        }

        if(zoemer == 1 && ((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 2.0) {
            deurBelUit();
        }

        knopValuePrev = knopValue;
    }

    close(sock);
}

void Zuil::noodAlarm(int n)
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    if(n) strcpy(buff, "noodAlarmAan\r");
    else strcpy(buff, "noodAlarmUit\r");
    sendMsg(buff);

    nood = n;
}

void Zuil::brandAlarm(int n)
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    if(n) strcpy(buff, "brandAlarmAan\r");
    else strcpy(buff, "brandAlarmUit\r");
    sendMsg(buff);

    brand = n;
}

void Zuil::deurBelAan()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "deurBelAan\r");
    sendMsg(buff);

    zoemer = 1;
    timer = clock();
}

void Zuil::deurBelUit()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "deurBelUit\r");
    sendMsg(buff);

    zoemer = 0;
}

json Zuil::getStatus()
{
    json zuilData = {{"Zoemer", zoemer}, {"Nood", nood}, {"Brand", brand}};

    return zuilData;
}
