#include "Deur.hpp"
#include "Zuil.hpp"

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

        sendMsg("getStatus\r");

        memset(buffer, 0, sizeof(buffer));
        if(recv(sock, buffer, 255, 0) < 1) { // dit wordt een functie
            std::cout << "Deur disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        /* try and catch json parse exceptions */

        try {
            auto j_deur = json::parse(buffer);

            knopBinnen = j_deur.at("binnenKnop");
            knopBuiten = j_deur.at("buitenKnop");
        }
        catch(json::exception& e) {
            std::cout << "Exception error at Deur: " << e.what() << std::endl;
        }

        /* state machine */

        switch(state) {
            case OPEN:
                if(knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
                    sluitDeur();
                }
                break;
            case DICHT:
                if(knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
                    openDeur();
                }
                break;
        }

        /* checks */

        if (knopBuiten == 1 && knopBuitenPrev != knopBuiten) {
            deurBel();
            buitenLampAan();
        }

        if (ledBuiten == 1 && ((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 5.0) {
            buitenLampUit();
        }

        knopBinnenPrev = knopBinnen;
        knopBuitenPrev = knopBuiten;
    }

    close(sock);
}

void Deur::openDeur()
{
    sendMsg("deurOpen\r");

    state = OPEN;
}

void Deur::sluitDeur()
{
    sendMsg("deurDicht\r");

    state = DICHT;
}

void Deur::deurBel()
{
    Zuil * zuil = dynamic_cast<Zuil *>(a->devices.find('f')->second);
    zuil->deurBelAan();
}

void Deur::buitenLampAan()
{
    sendMsg("buitenLampAan\r");

    ledBuiten = 1;
    timer = std::clock();
}

void Deur::buitenLampUit()
{
    sendMsg("buitenLampUit\r");

    ledBuiten = 0;
    timer = 0;
}

void Deur::binnenLampAan()
{
    sendMsg("binnenLampAan\r");

    ledBinnen = 1;
}

void Deur::binnenLampUit()
{
    sendMsg("binnenLampUit\r");

    ledBinnen = 0;
}

json Deur::getStatus()
{
    json deurData;
    deurData["Deur"] = {{"Deur", state ? "open" : "dicht"}, {"Binnenknop", knopBinnen}, {"Buitenknop", knopBuiten}, {"Binnenled", ledBinnen}, {"Buitenled", ledBuiten}};

    return deurData;
}
