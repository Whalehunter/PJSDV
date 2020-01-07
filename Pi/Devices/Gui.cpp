#include "Gui.hpp"
#include <string>

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
        std::cout << this << std::endl;

        if(buffer[0] == 'b' && a->bed != 0) {
            strcpy(buffer, "teringjong");
            a->bed->sendMsg(buffer);
        } else if(buffer[0] == 'd' && a->bed != 0) {
            int bedKnoppie = a->bed->getStatus();
            sprintf(buffer, "%d", bedKnoppie);
            sendMsg(buffer);
            std::cout << "knopwaarde van bed = " << bedKnoppie << std::endl;
        } else if(buffer[0] == 'x' && a->deur != 0) {
            if (buffer[1] == 'o') {
                a->deur->openDeur();
            } else {
                a->deur->sluitDeur();
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
