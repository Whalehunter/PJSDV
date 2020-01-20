#include "Schemerlamp.hpp"
#include <ctime>
#define COCKS_PER_SEC CLOCKS_PER_SEC

using json = nlohmann::json;

Schemerlamp::Schemerlamp(int n, Appartement* a): Device(n, a), disco(false), discoTimer(0)
{
    std::cout << "Schemerlamp aangemaakt" << std::endl;
    lamp = (*new RGBLed());
}

Schemerlamp::~Schemerlamp()
{}

void Schemerlamp::operator()()
{
    while (1) {
        if (!updateStatus()) break;

        if (isDisco() && ((std::clock() - discoTimer) / (double) COCKS_PER_SEC) >= 0.5) {
            json msg = lamp.getKleur(isDisco());
            sendMsg((msg.dump()+"\r").c_str());
            lamp.updateDiscoColor();
        }

        /* reset 5min timer als beweging */
        if (isDisco() && beweging) {
            activityTimer = std::clock();
        }

        /* als 5 min voorbij zonder beweging -> disco uit */
        if (isDisco() && ((std::clock() - activityTimer) / (double) COCKS_PER_SEC) >= 300) {
            setDisco(false);
            activityTimer = 0;
        }
    }
    close(sock);
    std::cout << "Schemerlamp connection closed" << std::endl;
}

bool Schemerlamp::isDisco() {
    return disco;
}

void Schemerlamp::setDisco(bool d) {
    if (disco && !d) {
        json msg = {{"R", 255},{"G", 255},{"B", 255}};
        sendMsg((msg.dump()+"\r").c_str());
        activityTimer = 0;
    } else if (!disco && d) {
        json msg = {{"R", 0},{"G", 0},{"B", 0}};
        sendMsg((msg.dump()+"\r").c_str());
        discoTimer = std::clock();
        activityTimer = std::clock();
    }
    disco = d;
}

void Schemerlamp::setKleur(int r, int g, int b) {
    lamp.setKleur(r,g,b);
    json kleur = lamp.getKleur();
    kleur["cmd"] = "kleur";
    sendMsg((kleur.dump()+"\r").c_str());
}

void Schemerlamp::uit() {
    lamp.uit();
    sendMsg((lamp.getKleur().dump()+"\r").c_str());
}

void Schemerlamp::aan() {
    lamp.aan();
    sendMsg((lamp.getKleur().dump()+"\r").c_str());
}

bool Schemerlamp::updateStatus()
{
    char buf[256];
    sendMsg("getStatus\r");
    memset(buf, 0, sizeof(buf));

    if (recv(sock, buf, 255, 0) < 1) {
        std::cout << "Schemerlamp is #gone" << std::endl;
        close(sock);
        return false;
    }

    try {
        auto jSL = json::parse(buf);
        lamp.setKleur(jSL.at("rood"), jSL.at("groen"), jSL.at("blauw"));
        beweging = jSL.at("beweging");
    }
    catch(json::exception& e) {
        std::cout << "Exception error at Deur: " << e.what() << std::endl;
    }
    return true;
}

json Schemerlamp::getStatus()
{
    json schemerlampie;
    schemerlampie["Schemerlamp"] = {{"Rood", lamp.rgb->red}, {"Groen", lamp.rgb->green}, {"Blauw", lamp.rgb->blue}, {"Beweging", beweging}, {"Disco", disco}};
    return schemerlampie;
}
