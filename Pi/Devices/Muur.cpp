#include "Muur.hpp"
#include "Deur.hpp"
#include <string>

using json = nlohmann::json;
using namespace std;

Muur::Muur(int n, Appartement* ap): Device(n, ap), ldr(0), pot(0), disco(false)
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
	/* get and store JSON values, break from while if Muur is gone */
        if (!updateStatus()) {
            break;
        }

        /* Indien LDR hoger is dan 500 en er is geen user input (ldrOverride), lamp uit en raam zichtbaar houden */
        if (ldr >= 500 && !ldrOverride) {
            setLCD(0);
	    /* kijk of deur bestaat, zo ja: cast deur pointer en call functie */
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->setBinnenLamp(false);
            }
        }
	/* dim LCD als <500 en er is geen user input */
        else if (!ldrOverride) {
            setLCD(1);
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->setBinnenLamp(true);
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

    if (isDisco() && compareTime(timer, 0.5)) {
        timer = std::clock();
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

void Muur::setLCD(int x)
{
    /* lock the mutex to ensure only one thread has access to raam */
    const std::lock_guard<std::mutex> lock(LCD_mutex);
    raam = x;
    /* LCD_mutex is automatically unlocked upon leaving this function */
}

bool Muur::isDisco()
{
    return disco;
}

void Muur::setRGB(bool x)
{
    /* lock the mutex to protect the critical section */
    const std::lock_guard<std::mutex> lock(RGB_mutex);
    if (x) {
    for (int i=0;i<LAMPEN;i++) lampen[i].aan();
    } else {
	for (int i=0;i<LAMPEN;i++) lampen[i].uit();
    }
    /* RGB_mutex is automatically unlocked upon leaving this function */
}

void Muur::setDisco(bool run)
{
    /* lock the mutex to protect the critical section */
    const std::lock_guard<std::mutex> lock(disco_mutex);
    if (disco && !run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(255,255,255);
        }
        socket.sendBuffer(arduinoStatus().c_str());
    } else if (!disco && run) {
        for (int i=0;i<LAMPEN;i++) {
            lampen[i].setKleur(0,0,0);
        }
        timer = std::clock();
        socket.sendBuffer(arduinoStatus().c_str());
    }
    disco = run;
    /* mutex is automatically unlocked upon leaving this function */
}

void Muur::setBrightness(bool up)
{
    /* lock the mutex to protect the critical section */
    const std::lock_guard<std::mutex> lock(brightness_mutex);
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
    /* mutex is automatically unlocked upon leaving this function */
}
