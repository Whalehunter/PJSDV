#include "Zuil.hpp"
#include "Deur.hpp"

using json = nlohmann::json;

Zuil::Zuil(int n, Appartement* ap): Device(n, ap), nood(0), brand(0), zoemer(0), timer(0)
{
    std::cout << "Zuil aangemaakt" << std::endl;
}

Zuil::~Zuil()
{}

void Zuil::operator()()
{
    char buffer[256] = {0};

    while (1) {
        sendMsg("getStatus\r");

        if(!recvMsg(buffer)) {
            std::cout << "Zuil disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        try {
            auto j_zuil = json::parse(buffer);

            knopValue = j_zuil.at("knopValue");
            sensorValue = j_zuil.at("sensorValue");
        }
        catch (json::exception& e) {
            std::cout << "Exception error at Zuil: " << e.what() << std::endl;
        }

        if (getKnop()) {
            noodAlarmAan();
        }
        if (getSensor() >= 920) {
            brandAlarmAan();
        }
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
    if (a->devices.count('d')) {
        dynamic_cast<Deur *>(a->devices.find('d')->second)->noodKnipperAan();
    }
}

void Zuil::brandAlarmUit()
{
    if (!nood && !timer)
        zoemerUit();
    brand = 0;
    if (a->devices.count('d')) {
        dynamic_cast<Deur *>(a->devices.find('d')->second)->noodKnipperUit();
    }
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

void Zuil::zoemerAan()
{
    sendMsg("zoemerAan\r");
    zoemer = 1;
}

void Zuil::zoemerUit()
{
    sendMsg("zoemerUit\r");
    zoemer = 0;
}

json Zuil::getStatus()
{
    json zuilData;
    zuilData["Zuil"] = {{"Zoemer", zoemer}, {"Noodalarm", nood}, {"Brandalarm", brand}, {"Gasmeter", sensorValue}, {"Knop", knopValue}};

    return zuilData;
}
