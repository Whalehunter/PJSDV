#include "Gui.hpp"
#include "Deur.hpp"
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
    int bedKnoppie = 0;
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::cout << buffer << std::endl;

        if(buffer[0] == 'b' && a->bed != 0) {
            strcpy(buffer, "teringjong");
            a->bed->sendMsg(buffer);
        } else if(buffer[0] == 'd' && a->bed != 0) {
            int bedKnoppie = a->bed->getStatus();
            sprintf(buffer, "%d", bedKnoppie);
            sendMsg(buffer);
            std::cout << "knopwaarde van bed = " << bedKnoppie << std::endl;
        } else if(buffer[0] == 'x') {
            Deur * deur = dynamic_cast<Deur *>(a->devices.find('d')->second);
            std::cout << "1" << std::endl;
            if (buffer[1] == 'o') {
                std::cout << "2" << std::endl;
                sendMsg(buffer);
                deur->openDeur();

            } else {
                deur->sluitDeur();
                sendMsg(buffer);
            }
        }

        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

int Gui::getStatus()
{
    return knopValue; // placeholder, we gaan hoop ik JSON gebruiken
}
