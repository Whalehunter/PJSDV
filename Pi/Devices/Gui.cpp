#include "Gui.hpp"

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
    int timestamp = 0;
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::cout << a->bed << std::endl;
        timestamp++;

        if(buffer[0] == 'b' && a->bed != 0) {
            a->bed->sendMsg(buffer);
        }

        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

void Gui::getStatus()
{
    std::cout << "anders" << std::endl;
}
