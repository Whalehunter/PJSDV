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

Gui::Gui(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "gui aangemaakt" << std::endl;
}

Gui::~Gui()
{}

void Gui::operator()()
{
    char buffer[256] = {0};     // initializeer lege buffer

    while (recvMsg(buffer)) {    // Voor elk ontvangen bericht
        std::cout << buffer << std::endl;
        char *p = buffer;

        /* loop through string and call getStatus on found devices */
        /* append getStatus() and send to GUI as json */
        if (*p == '-') {
	    /* MUTEX LOCK REQ */
            json deviceStatus;
            while(*p)
                if (a->devices.count(*(++p)))
                    deviceStatus.push_back(a->devices.find(*p)->second->getStatus());
            sendMsg(deviceStatus.dump().c_str());
        }

        else if (*p == 'z' && a->devices.count(*p)) { // Stoel
            Stoel * stoel = dynamic_cast<Stoel*>(a->devices.find(*p++)->second);
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
            sendMsg("{\"success\":true}");
        }

        else if (*p == 'd' && a->devices.count(*p)) { // Deur
            Deur * deur = dynamic_cast<Deur *>(a->devices.find(*p++)->second);
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
            sendMsg("{\"success\":true}");
        }

        else if (*p == 'f' && a->devices.count(*p)) { // Zuil
            Zuil * zuil = dynamic_cast<Zuil *>(a->devices.find(*p++)->second);
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
            sendMsg("{\"success\":true}");
        }

        else if (*p == 'k' && a->devices.count(*p)) { // Koelkast
            dynamic_cast<Koelkast *>(a->devices.find(*p)->second)->disableKoelAlarm();
        }

        else if (*p == 'm' && a->devices.count(*p)) { // Muur
            Muur * muur = dynamic_cast<Muur *>(a->devices.find(*p++)->second);
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
            sendMsg("{\"success\":true}");
        }

        else if (*p == 's' && a->devices.count(*p)) { // schemerlamp
            Schemerlamp * schemerlamp = dynamic_cast<Schemerlamp *>(a->devices.find(*p++)->second);
            if (*p == 'l') {    // lamp aan/uit
                if (*(++p) == 'a') schemerlamp->aan();
                else schemerlamp->uit();
            }
            else if (*p == 'd') { // disco aan/uit
                if (*(++p) == 'a') schemerlamp->setDisco(true);
                else schemerlamp->setDisco(false);
            }
            sendMsg("{\"success\":true}");
        }

        else if (*p == 'y' && a->devices.count(*p)) { // Bed
            Bed * bed = dynamic_cast<Bed *>(a->devices.find(*p++)->second);
            if (*p == 'l') {    // lamp aan/uit
                if (*(++p) == 'a') bed->ledAan();
                else bed->ledUit();
            }
            sendMsg("{\"success\":true}");
        }

        memset(buffer, 0, sizeof(buffer)); // reset buffer
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Gui::getStatus()
{
    return json::object();
}
