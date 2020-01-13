#include "Schemerlamp.hpp"

using json = nlohmann::json;

Schemerlamp::Schemerlamp(int n, Appartement* a): Device(n, a)
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
    }
    close(sock);
    std::cout << "Schemerlamp connection closed" << std::endl;
}

void Schemerlamp::setKleur(int r, int g, int b) {
    lamp.setKleur(r,g,b);
    json kleur = lamp.getKleur();
    kleur["cmd"] = "kleur";
    sendMsg(kleur.dump().c_str());
}

void Schemerlamp::uit() {
    if (lamp.isOn()) {
        lamp.toggle();
    }
}

void Schemerlamp::aan() {
    if (lamp.isOff()) {
        lamp.toggle();
    }
}

json Schemerlamp::getStatus()
{
    json schemerlampie;
    schemerlampie["Schemerlamp"] = {{"Rood", lamp.rgb->red}, {"Groen", lamp.rgb->green}, {"Blauw", lamp.rgb->blue}, {"Beweging", beweging}};
    return schemerlampie;
}
