#include "Gui.hpp"
#include "Deur.hpp"
#include "Zuil.hpp"
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

        if(*p == '-') {
            while(*p)
                if(a->devices.count(*(++p)))
                    sendMsg(a->devices.find(*p)->second->getStatus().dump().c_str());
        }

        else if(*buffer == 'x') {
            Deur * deur = dynamic_cast<Deur *>(a->devices.find('d')->second);
            //    char *p = buffer;
            if (*(++p) == 'o') {
                sendMsg(buffer);
                deur->openDeur();

            } else {
                deur->sluitDeur();
                sendMsg(buffer);
            }
        }

        else if(*p == 'f') {
            Zuil * zuil = dynamic_cast<Zuil *>(a->devices.find(*p++)->second);

            if(*p == 'n') {
                if (*(++p) == 'a') zuil->noodAlarmAan();
                else zuil->noodAlarmUit();

                sendMsg(buffer);
            }
            /* else if(*p == 'b') {
               if (*(++p) == 'a') zuil->brandAlarmAan();
               else zuil->brandAlarmUit();
               }*/
        }

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
