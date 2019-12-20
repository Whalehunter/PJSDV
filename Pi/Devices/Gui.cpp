#include "Gui.hpp"
#include <string>

Gui::Gui(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "gui aangemaakt" << std::endl;
}

Gui::~Gui()
{
}

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
        }

        if(buffer[0] == 'd' && a->bed != 0) {
      //      strcpy(buffer, "lampAan\r");
      //      a->bed->sendMsg(buffer);

      //      memset(buffer, 0, sizeof(buffer));

            int bedKnoppie = a->bed->getStatus();
            sprintf(buffer, "%d", bedKnoppie);
            sendMsg(buffer);
            std::cout << "knopwaarde van bed = " << bedKnoppie << std::endl;
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