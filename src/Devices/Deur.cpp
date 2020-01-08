#include "Deur.hpp"

using json = nlohmann::json;

Deur::Deur(int n, Appartement* ap): Device(n, ap), state(DICHT), knopBinnen(0), knopBuiten(0), ledBinnen(0), ledBuiten(0), timer(0)
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

    while(1) {
        /* get and store JSON values */

        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);

        memset(buffer, 0, sizeof(buffer));
        if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
            std::cout << "Deur disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        /* try and catch json parse exceptions */

        try {
            auto j_deur = json::parse(buffer); // hier moeten ook exceptions afgehandeld worden

            knopBinnen = j_deur.at("binnenKnop");
            knopBuiten = j_deur.at("buitenKnop");
        }
        catch(json::parse_error) {
            std::cout << "Parsing error at Deur on socket " << sock << std::endl;
        }

       // knopBinnen = j_deur.at("binnenKnop");
       // knopBuiten = j_deur.at("buitenKnop");

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

        /* checks */

        if (knopBuiten == 1 && knopBuitenPrev != knopBuiten) {
            deurBel();
            buitenLampAan();
        }

        if (ledBuiten == 1 && (std::clock() - timer / (double) CLOCKS_PER_SEC) >= 5.0) {
            buitenLampUit();
        }

        knopBinnenPrev = knopBinnen;
        knopBuitenPrev = knopBuiten;
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

void Deur::buitenLampAan()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "buitenLampAan\r");
    sendMsg(buff);

    ledBuiten = 1;
    timer = std::clock();
}

void Deur::buitenLampUit()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "buitenLampUit\r");
    sendMsg(buff);

    ledBuiten = 0;
    timer = 0;
}

json Deur::getStatus()
{
    json deurData = {{"State", state}, {"Binnenknop", knopBinnen}, {"Buitenknop", knopBuiten}, {"Binnenled", ledBinnen}, {"Buitenled", ledBuiten}};

    return deurData;
}
