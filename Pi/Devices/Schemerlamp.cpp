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

    while(1) {
        char buf[256];
        strcpy(buf, "getStatus\r");
        sendMsg(buf);
        memset(buf, 0, sizeof(buf));

        if (recv(sock, buf, 255, 0) < 1) {
            std::cout << "Schemerlamp is #gone" << std::endl;
            close(sock);
            return;
        }

        try {
            auto jSL = json::parse(buf);
            lamp.setKleur(jSL.at("rood"), jSL.at("groen"), jSL.at("blauw"));
            beweging = jSL.at("beweging");
        }
        catch(json::exception& e) {
            std::cout << "Exception error at Deur: " << e.what() << std::endl;
        }

        if (isDisco() && ((std::clock() - discoTimer) / (double) COCKS_PER_SEC) >= 0.5) {
            json msg = {{"R", getDiscoKleur("rood")},{"G", getDiscoKleur("groen")},{"B", getDiscoKleur("blauw")},};
            sendMsg((msg.dump()+"\r").c_str());
            updateDiscoColor();
        }
    }
    close(sock);
    std::cout << "Schemerlamp connection closed" << std::endl;
}

void Schemerlamp::updateDiscoColor() {
    if (currentDiscoColor == "rood") {
        currentDiscoColor = "groen";
    } else if (currentDiscoColor == "groen") {
        currentDiscoColor = "blauw";
    } else {
        currentDiscoColor = "rood";
    }
}

bool Schemerlamp::isDisco() {
    return disco;
}

void Schemerlamp::setDisco(bool d) {
    if (disco && !d) {
        json msg = {{"R", 255},{"G", 255},{"B", 255}};
        sendMsg((msg.dump()+"\r").c_str());
    } else if (!disco && d) {
        json msg = {{"R", 0},{"G", 0},{"B", 0}};
        sendMsg((msg.dump()+"\r").c_str());
        discoTimer = std::clock();
    }
    disco = d;
}

int Schemerlamp::getDiscoKleur(std::string kleur) {
    if (this->currentDiscoColor == kleur) {
        discoTimer = std::clock();
        return 255;
    }

    return 0;
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

json Schemerlamp::getStatus()
{
    json schemerlampie;
    schemerlampie["Schemerlamp"] = {{"Rood", lamp.rgb->red}, {"Groen", lamp.rgb->green}, {"Blauw", lamp.rgb->blue}, {"Beweging", beweging}, {"Disco", disco};
    return schemerlampie;
}
