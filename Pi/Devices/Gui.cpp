#include "Gui.hpp"
#include "Deur.hpp"
#include "Zuil.hpp"
#include "Schemerlamp.hpp"
#include "Stoel.hpp"
#include "Bed.hpp"
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
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::cout << buffer << std::endl;
        char *p = buffer;

        /* loop through string and call getStatus on found devices */
        /* append getStatus returns and send to GUI as json */
        if(*p == '-') {
            json deviceStatus;
               /*for(std::map<char, Device*>::iterator i = a->devices.begin(); i != a->devices.end(); ++i) {
                 deviceStatus.push_back(i->second->getStatus());
                 }
                 sendMsg(deviceStatus.dump().c_str());*/
            while(*p)
                if(a->devices.count(*(++p))) {
                    deviceStatus.push_back(a->devices.find(*p)->second->getStatus());
                }
            sendMsg(deviceStatus.dump().c_str());

        }

        else if (*p == 'z' && a->devices.count(*p)) {
            Stoel * stoel = dynamic_cast<Stoel*>(a->devices.find(*p++)->second);

            if (*p == 't') {    // trillen
                if (*(++p) == 'a') {
                    stoel->trilAan();
                } else {
                    stoel->trilUit();
                }
            } else if (*p == 'l') {   // lamp
                if (*(++p) == 'a') {
                    stoel->ledAan();
                } else {
                    stoel->ledUit();
                }
            }
            sendMsg("{\"success\":true}");
        }

        else if(*p == 'd' && a->devices.count(*p)) {
            Deur * deur = dynamic_cast<Deur *>(a->devices.find(*p++)->second);

            if (*p == 'o') {
                deur->openDeur();
            }
            else if (*p == 's') {
                deur->sluitDeur();
            }
            else if (*p == 'l') { // dla voor binnenlamp
                if (*(++p) == 'a') deur->binnenLampAan();
                else deur->binnenLampUit();
            }
            else if (*p == 'b') { // dba voor buitenlamp
                if (*(++p) == 'a') deur->buitenLampAan();
                else deur->buitenLampUit();
            }
            sendMsg("{\"success\":true}");
        }

        else if(*p == 'f' && a->devices.count(*p)) {
            Zuil * zuil = dynamic_cast<Zuil *>(a->devices.find(*p++)->second);
            if(*p == 'n') {
                if (*(++p) == 'a') zuil->noodAlarmAan();
                else zuil->noodAlarmUit();
            }
            else if(*p == 'b') {
                if (*(++p) == 'a') zuil->brandAlarmAan();
                else zuil->brandAlarmUit();
            }
            else if(*p == 'z') {
                if (*(++p) == 'a') zuil->zoemerAan();
                else zuil->zoemerUit();
            }
            sendMsg("{\"success\":true}");
        }

        else if(*p == 's' && a->devices.count(*p)) {
            Schemerlamp * schemerlamp = dynamic_cast<Schemerlamp *>(a->devices.find(*p++)->second);
            if (*p == 'l') {
                if (*(++p) == 'a') schemerlamp->aan();
                else schemerlamp->uit();
            }
            else if (*p == 'd') {
                if (*(++p) == 'a') schemerlamp->setDisco(true);
                else schemerlamp->setDisco(false);
            }
            sendMsg("{\"success\":true}");
        }

        else if (*p == 'y' && a->devices.count(*p)) {
            Bed * bed = dynamic_cast<Bed *>(a->devices.find(*p++)->second);
            if (*p == 'l') {
                if (*(++p) == 'a') bed->ledAan();
                else bed->ledUit();
            }
        }

        // sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

json Gui::getStatus()
{
    json data = {"Placeholder", 0};
    return data; // placeholder, we gaan hoop ik JSON gebruiken
}
