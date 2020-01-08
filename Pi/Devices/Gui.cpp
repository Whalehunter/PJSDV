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
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::cout << buffer << std::endl;
        if(*buffer == 'b' && a->bed != 0) {
            strcpy(buffer, "teringjong");
            a->bed->sendMsg(buffer);
        } else if(*buffer == 'd') {
            sendMsg(a->devices.find(*buffer)->second->getStatus().dump().c_str());
        } else if(*buffer == 'x') {
            Deur * deur = dynamic_cast<Deur *>(a->devices.find('d')->second);
            std::cout << "1" << std::endl;
            if (++*buffer == 'o') {
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

json Gui::getStatus()
{
    json data = {"Placeholder", 0};
    return data; // placeholder, we gaan hoop ik JSON gebruiken
}
