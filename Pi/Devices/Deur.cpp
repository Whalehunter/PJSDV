#include "Deur.hpp"

using json = nlohmann::json;

Deur::Deur(int n, Appartement* ap): Device(n, ap), state(DICHT), knopBinnen(0), knopBuiten(0), ledBinnen(0), ledBuiten(0)
{
    std::cout << "Deur aangemaakt" << std::endl;
}

Deur::~Deur()
{
}

void Deur::operator()()
{
    char buffer[256];
    int knopBinnenPrev = 0;
    int knopBuitenPrev = 0;
    //clock_t t;

    while(1) {
       // std::cout << getDeurStatus() << std::endl;

        /* get and store JSON values */

        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);

        memset(buffer, 0, sizeof(buffer));
        if(recv(sock, buffer, 255, 0) < 1) { // voor nu even geen functie van gemaakt, wordt geintegreerd in Device
            std::cout << "Deur disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

       // std::cout << buffer << std::endl;

        auto j_deur = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

        knopBinnen = j_deur.at("binnenKnop");
        knopBuiten = j_deur.at("buitenKnop");

        /* state machine */

        switch(state) {
            case OPEN:
                if(knopBinnen == 1 && knopBinnenPrev != knopBinnen) {
                    sluitDeur();
                } 
                break;
            case DICHT:
                if(knopBinnen == 1 && knopBinnenPrev != knopBinnen) {
                    openDeur();
                }
                break;
        }

        if (knopBuiten == 1 && knopBuitenPrev != knopBuiten) {
            deurBel();
        }

        knopBinnenPrev = knopBinnen;
        knopBuitenPrev = knopBuiten;

    //    std::cout << getDeurStatus() << std::endl;
    }

   close(sock);
}

void Deur::openDeur()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "deurOpen\r");
    sendMsg(buff);

    state = OPEN;
}

void Deur::sluitDeur()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "deurDicht\r");
    sendMsg(buff);

    state = DICHT;
}

void Deur::deurBel()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "deurBel\r");
    sendMsg(buff);
}

json Deur::getDeurStatus()
{
    json deurData = {{"State", state}, {"Binnenknop", knopBinnen}, {"Buitenknop", knopBuiten}, {"Binnenled", ledBinnen}, {"Buitenled", ledBuiten}};

    return deurData;
}

int Deur::getStatus()
{
    return state; // placeholder
}

