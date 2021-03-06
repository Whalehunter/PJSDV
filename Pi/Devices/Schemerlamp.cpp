#include "Schemerlamp.hpp"
#include <ctime>

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
        /* update status and check if device is still connected, break if disconnected */
        if (!updateStatus()) break;

        /* verander disco kleurtjes om de 0.5s */
        if (isDisco() && compareTime(discoTimer, 0.5)) {
            json msg = lamp.getKleur(isDisco());
            socket.sendBuffer((msg.dump()+"\r").c_str());
            lamp.updateDiscoColor();
        }

        /* reset 5min timer als beweging */
        if (isDisco() && beweging) {
            activityTimer = std::clock();
        }

        /* als 5 min voorbij zonder beweging -> disco uit */
        if (isDisco() && compareTime(activityTimer, 300.0)) {
            setDisco(false);
            activityTimer = 0;
        }
    }
    /* close connectie voordat de thread sluit */
    close(socket.getId());
    std::cout << "Schemerlamp connection closed" << std::endl;
}

bool Schemerlamp::isDisco()
{
    return disco;
}

void Schemerlamp::setDisco(bool d)
{
    /* lock mutex so only one thread can access critical data at a time */
    const std::lock_guard<std::mutex> lock (kleur_mutex);
    /* if disco = on and input = false -> turn disco off
     * if disco = off and input = true -> turn disco on */
    if (disco && !d) {
        json msg = {{"R", 255},{"G", 255},{"B", 255}};
        socket.sendBuffer((msg.dump()+"\r").c_str());
        activityTimer = 0;
    } else if (!disco && d) {
        json msg = {{"R", 0},{"G", 0},{"B", 0}};
        socket.sendBuffer((msg.dump()+"\r").c_str());
        discoTimer = std::clock();
        activityTimer = std::clock();
    }
    disco = d;
    /* mutex is automatically unlocked upon leaving its scope */
}

/**
 * Kleur updaten van RGB lED
 *
 * param r: Rode kleur
 * param g: Groene kleur
 * param b: Blauwe kleur
 *
 * return void
 */
void Schemerlamp::setKleur(int r, int g, int b)
{
    /* lock mutex */
    const std::lock_guard<std::mutex> lock (kleur_mutex);
    lamp.setKleur(r,g,b);
    json kleur = lamp.getKleur();
    kleur["cmd"] = "kleur";
    socket.sendBuffer((kleur.dump()+"\r").c_str());
}

void Schemerlamp::setLamp(bool x)
{
    /* lock mutex */
    const std::lock_guard<std::mutex> lock (lamp_mutex);
    if (x) {
        lamp.aan();
    } else {
        lamp.uit();
    }
    socket.sendBuffer((lamp.getKleur().dump()+"\r").c_str());
}

bool Schemerlamp::updateStatus()
{
    char buf[256] = {0};
    socket.sendBuffer("getStatus\r");
    if (!socket.receiveBuffer(buf)) {
        std::cout << "Schemerlamp is #gone" << std::endl;
        close(socket.getId());
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
