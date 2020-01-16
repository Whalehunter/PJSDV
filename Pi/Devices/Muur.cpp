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
    while(1) {
        if (!updateStatus()) {
            break;
        }

        /* Indien LDR hoger is dan 500, lamp uit en raam zichtbaar houden */
        if (ldr >= 500) {
            LCDdoorlaten();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampUit();
            }
        }
        else {
            LCDdimmen();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampAan();
            }

        }
        /*Sends value of pot to WEMOS*/
        RGBdimmen();
    }
}

nlohmann::json Muur::getStatus()
{
    json muurData;
    muurData["Muur"] = {{"LDR", ldr}, {"POT", pot}, {"Raam", raam}};
    for (int i=0;i<LAMPEN;i++){
        muurData["Muur"]["LED" + std::to_string(i)] = lampen[i].getKleur();
    }

    return muurData;
}

bool Muur::updateStatus()
{
    char buffer[300] ={};
    sendMsg("getStatus\r");

//    memset(buffer, 0, sizeof(buffer));
    if(recv(sock, buffer, 299, 0) < 1){
        std::cout << "Muur disconnected from socket: " << sock << std::endl;
        close(sock);
        return false;
    }
    std::cout << buffer << std::endl;

    try {
        auto j_muur = json::parse(buffer);

        ldr = j_muur.at("ldr");
        pot = j_muur.at("pot");
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setBrightness(pot/4);
            json o = j_muur.at(std::to_string(i));
            lampen[i].setKleur(o.at("r"), o.at("g"), o.at("b"));
            sendMsg(arduinoStatus());
        }

        if (isDisco() && ((std::clock() - discoTimer) / (double) CLOCKS_PER_SEC) >= 0.5) {
            json msg = json::object();
            for (int i=0;i<LAMPEN;i++) {
                std::string name = "LED" + std::to_string(i);
                msg[name] = lampen[i].getKleur(isDisco());
                discoTimer = std::clock();
            }
            msg["S"] = raam;
            sendMsg((msg.dump()+"\r").c_str());
        }
    }
    catch(json::exception& e){
        std::cout << "Parsing error: " << e.what() << std::endl;
    }

    return true;
}

const char * Muur::arduinoStatus() {
    json msg = json::object();
    for (int i=0;i<LAMPEN;i++) {
        std::string name = "LED" + std::to_string(i);
        msg[name] = lampen[i].getKleur();
    }
    msg["S"] = raam;
    return (msg.dump()+"\r").c_str();
}

void Muur::LCDdimmen()
{
    raam = 1;
    sendMsg(arduinoStatus());
}

void Muur::LCDdoorlaten()
{
    raam = 0;
    sendMsg(arduinoStatus());
}

bool Muur::isDisco() {
    return disco;
}

void Muur::RGBdimmen()
{
    if (pot == 0){
        RGBuit();
    }
    else {
        RGBaan();
    }
}

void Muur::RGBaan()
{
    for (int i=0;i<LAMPEN;i++) {
        lampen[i].aan();
    }
    sendMsg(arduinoStatus());
}

void Muur::RGBuit()
{
    for (int i=0;i<LAMPEN;i++) {
        lampen[i].uit();
    }
    sendMsg(arduinoStatus());
}

void Muur::setDisco(bool run)
{
    if (disco && !run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(255,255,255);
        }
        sendMsg(arduinoStatus());
    } else if (!disco && run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(0,0,0);
        }
        discoTimer = std::clock();
        sendMsg(arduinoStatus());
    }
    disco = run;
}
