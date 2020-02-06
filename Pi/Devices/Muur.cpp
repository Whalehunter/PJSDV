#include "Muur.hpp"
#include "Deur.hpp"
#include <string>

using json = nlohmann::json;
using namespace std;

Muur::Muur(int n, Appartement* ap): Device(n, ap), ldr(0), pot(0), disco(false), discoTimer(0)
{
    std::cout << "Muur aangemaakt" << std::endl;

    // Initializeer leds
    lampen[0] = RGBLed();
    lampen[1] = RGBLed();
    lampen[1].currentDiscoColor = "groen";
    lampen[2] = RGBLed();
    lampen[2].currentDiscoColor = "blauw";
}

Muur::~Muur()
{}

void Muur::operator ()()
{
    while (1) {
        if (!updateStatus()) {
            break;
        }

        /* Indien LDR hoger is dan 500, lamp uit en raam zichtbaar houden */
        if (ldr >= 500 && !ldrOverride) {
            LCDdoorlaten();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampUit();
            }
        }
        else if (!ldrOverride) {
            LCDdimmen();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampAan();
            }
        }

        socket.sendBuffer(arduinoStatus().c_str());
    }
}

nlohmann::json Muur::getStatus()
{
    json muurData;
    muurData["Muur"] = {{"LDR", ldr}, {"POT", pot}, {"Raam", raam}, {"Disco", disco}};
    for (int i=0;i<LAMPEN;i++) {
        muurData["Muur"]["LED" + std::to_string(i)] = lampen[i].getKleur(isDisco());
    }

    return muurData;
}

bool Muur::updateStatus()
{
    char buffer[400] = {0};
    socket.sendBuffer("getStatus\r");

    if (!socket.receiveBuffer(buffer)) {
        int socketId = socket.getId();
        std::cout << "Muur disconnected from socket: " << socketId << std::endl;
        close(socketId);
        return false;
    }

    try {
        auto j_muur = json::parse(buffer);
        ldr = j_muur.at("ldr");
        pot = j_muur.at("pot");
        for (int i=0;i<LAMPEN;i++) {
            int currentPot = lampen[i].rgb->pot;
            if (currentPot - pot > 10 || currentPot - pot < -10)
                lampen[i].setBrightness(pot/4);
            json o = j_muur.at(std::to_string(i));
            lampen[i].setKleur(o.at("r"), o.at("g"), o.at("b"));
            lampen[i].rgb->pot = pot;
        }

    }
    catch (json::exception& e) {
        std::cout << "Parsing error: " << e.what() << std::endl;
    }

    if (isDisco() && ((std::clock() - discoTimer) / (double) CLOCKS_PER_SEC) >= 0.5) {
        discoTimer = std::clock();
        for (int i = 0; i < LAMPEN; i++) {
            lampen[i].updateDiscoColor();
        }
    }

    return true;
}

std::string Muur::arduinoStatus()
{
    json msg = json::object();
    for (int i=0;i<LAMPEN;i++) {
        std::string name = "LED" + std::to_string(i);
        msg[name] = lampen[i].getKleur(isDisco());
    }
    msg["S"] = raam;
    return (msg.dump()+"\r");
}

void Muur::LCDdimmen()
{
    raam = 1;
}

void Muur::LCDdoorlaten()
{
    raam = 0;
}

bool Muur::isDisco()
{
    return disco;
}

void Muur::RGBaan()
{
    for (int i=0;i<LAMPEN;i++) lampen[i].aan();
}

void Muur::RGBuit()
{
    for (int i=0;i<LAMPEN;i++) lampen[i].uit();
}

void Muur::setDisco(bool run)
{
    if (disco && !run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(255,255,255);
        }
        socket.sendBuffer(arduinoStatus().c_str());
    } else if (!disco && run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(0,0,0);
        }
        discoTimer = std::clock();
        socket.sendBuffer(arduinoStatus().c_str());
    }
    disco = run;
}

void Muur::setBrightness(bool up)
{
    int update = 25;
    for (int i=0;i<LAMPEN;i++) {
        int *b = &lampen[i].rgb->brightness;
        if (up) {
            if ((*b)+update <= 255) (*b) += update;
            else update = 255;
        } else {
            if ((*b)-update >= 0) (*b) -= update;
            else update =0;
        }
    }
}
