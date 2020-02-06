#include "Gui.hpp"
#include "Deur.hpp"
#include "Zuil.hpp"
#include "Schemerlamp.hpp"
#include "Stoel.hpp"
#include "Bed.hpp"
#include "Muur.hpp"
#include "Koelkast.hpp"
#include <string>

using json = nlohmann::json;

Gui::Gui(int n, std::map<char, Device*>* devices): devices(devices), socket(n, 256)
{
    std::cout << "gui aangemaakt" << std::endl;
}

Gui::~Gui()
{}

void Gui::operator()()
{
    char buffer[256] = {0};     // initializeer lege buffer
    int socketId = socket.getId();

    while (socket.receiveBuffer(buffer)) {    // Voor elk ontvangen bericht
        std::cout << buffer << std::endl;
        char *p = buffer;

        /* loop through string and call getStatus on found devices */
        /* append getStatus() and send to GUI as json */
        if (*p == '-') {
            json deviceStatus;
            while(*p)
                if (devices->count(*(++p)))
                    deviceStatus.push_back(devices->find(*p)->second->getStatus());
            socket.sendBuffer(deviceStatus.dump().c_str());
        }

        else if (*p == 'z' && devices->count(*p)) { // Stoel
            Stoel * stoel = dynamic_cast<Stoel*>(devices->find(*p++)->second);
            if (*p == 't') {    // Trillen aan/uit
                if (*(++p) == 'a') {
                    stoel->trilAan();
                } else {
                    stoel->trilUit();
                }
            } else if (*p == 'l') {   // Lamp aan/uit
                if (*(++p) == 'a') {
                    stoel->ledAan();
                } else {
                    stoel->ledUit();
                }
            }
            socket.sendBuffer("{\"success\":true}");
        }

        else if (*p == 'd' && devices->count(*p)) { // Deur
            Deur * deur = dynamic_cast<Deur *>(devices->find(*p++)->second);
            if (*p == 'o') {    // openen
                deur->openDeur();
            }
            else if (*p == 's') { // sluiten
                deur->sluitDeur();
            }
            else if (*p == 'l') { // binnen lamp aan/uit
                if (*(++p) == 'a') deur->binnenLampAan();
                else deur->binnenLampUit();
            }
            else if (*p == 'b') { // buiten lamp aan/uit
                if (*(++p) == 'a') deur->buitenLampAan();
                else deur->buitenLampUit();
            }
            socket.sendBuffer("{\"success\":true}");
        }

        else if (*p == 'f' && devices->count(*p)) { // Zuil
            Zuil * zuil = dynamic_cast<Zuil *>(devices->find(*p++)->second);
            if (*p == 'n') {     // nood alarm aan/uit
                if (*(++p) == 'a') zuil->noodAlarmAan();
                else zuil->noodAlarmUit();
            }
            else if (*p == 'b') { // brand alarm aan/uit
                if (*(++p) == 'a') zuil->brandAlarmAan();
                else zuil->brandAlarmUit();
            }
            else if (*p == 'z') { // zoemer aan/uit
                if (*(++p) == 'a') zuil->zoemerAan();
                else zuil->zoemerUit();
            }
            socket.sendBuffer("{\"success\":true}");
        }

        else if (*p == 'k' && devices->count(*p)) { // Koelkast
            dynamic_cast<Koelkast *>(devices->find(*p)->second)->disableKoelAlarm();
        }

        else if (*p == 'm' && devices->count(*p)) { // Muur
            Muur * muur = dynamic_cast<Muur *>(devices->find(*p++)->second);
            if (*p == 'd') {    // disco aan/uit
                if (*(++p) == 'a') muur->setDisco(true);
                else muur->setDisco(false);
            }
            else if (*p == 'l') { // lampen aan/uit
                if (*(++p) == 'a') muur->RGBaan();
                else muur->RGBuit();
            }
            else if (*p == 'r') { // raam dimmer aan/uit
                if (*(++p) == 'a') {
                    if (muur->ldrOverride) muur->ldrOverride = false;
                    else muur->ldrOverride = true;
                    muur->LCDdimmen();
                } else {
                    if (muur->ldrOverride) muur->ldrOverride = false;
                    else muur->ldrOverride = true;
                    muur->LCDdoorlaten();
                }
            }
            else if (*p == 'b') { // lamp helderheid +/-
                if (*(++p) == 'u') {
                    muur->setBrightness(true);
                } else {
                    muur->setBrightness(false);
                }
            }
            socket.sendBuffer("{\"success\":true}");
        }

        else if (*p == 's' && devices->count(*p)) { // schemerlamp
            Schemerlamp * schemerlamp = dynamic_cast<Schemerlamp *>(devices->find(*p++)->second);
            if (*p == 'l') {    // lamp aan/uit
                if (*(++p) == 'a') schemerlamp->aan();
                else schemerlamp->uit();
            }
            else if (*p == 'd') { // disco aan/uit
                if (*(++p) == 'a') schemerlamp->setDisco(true);
                else schemerlamp->setDisco(false);
            }
            socket.sendBuffer("{\"success\":true}");
        }

        else if (*p == 'y' && devices->count(*p)) { // Bed
            Bed * bed = dynamic_cast<Bed *>(devices->find(*p++)->second);
            if (*p == 'l') {    // lamp aan/uit
                if (*(++p) == 'a') bed->ledAan();
                else bed->ledUit();
            }
            socket.sendBuffer("{\"success\":true}");
        }

        memset(buffer, 0, sizeof(buffer)); // reset buffer
    }
    close(socketId);
    std::cout << "Connection closed on socket " << socketId << std::endl;
}
