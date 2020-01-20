#include "Deur.hpp"
#include "Zuil.hpp"

using json = nlohmann::json;

Deur::Deur(int n, Appartement* ap): Device(n, ap), state(DICHT), knopBinnen(0), knopBuiten(0), ledBinnen(0), ledBuiten(0), noodKnipper(0), timer(0), knipperTimer(0)
{
    std::cout << "Deur aangemaakt" << std::endl;
}

Deur::~Deur()
{}

void Deur::operator()()
{
    /* used to check previous values to avoid jittery button presses */
    int knopBinnenPrev = 0;
    int knopBuitenPrev = 0;

    while (1) {
        /* get and store JSON values, break from while if Deur is #gone */
        if (!updateStatus()) break;

        /* state machine, if state = closed && knop == pressed, open door, etc  */
        switch (state) {
        case OPEN:
            if (knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
                sluitDeur();
            }
            break;
        case DICHT:
            if (knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
                openDeur();
            }
            break;
        }

        /* operations based on checks */
        if (knopBuiten == 1 && knopBuitenPrev != knopBuiten) {
            deurBelAan();
            buitenLampAan();
        }
        else if (knopBuiten == 0 && knopBuitenPrev != knopBuiten) {
            deurBelUit();
        }

        /* turn external led off after 30 seconds */
        if (ledBuiten == 1 && ((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 30.0) {
            buitenLampUit();
        }

        /* if fire alarm, blink internal led */
        if (noodKnipper == 1 && ((std::clock() - knipperTimer) / (double) CLOCKS_PER_SEC) >= 1.0) {
            if (ledBinnen) {
                binnenLampUit(true);
                knipperTimer = std::clock();
            }
            else if (!ledBinnen) {
                binnenLampAan(true);
                knipperTimer = std::clock();
            }
        }

        /* store old value in Prev variables */
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

void Deur::deurBelAan()
{
    char cZuil = 'f';
    /* check if device exists, then call its function */
    /* dynamic cast to reach child functions */
    if (a->devices.count(cZuil))
        dynamic_cast<Zuil *>(a->devices.find(cZuil)->second)->deurBelAan();
}

void Deur::deurBelUit()
{
    char cZuil = 'f';
    if (a->devices.count(cZuil))
        dynamic_cast<Zuil *>(a->devices.find(cZuil)->second)->deurBelUit();
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

void Deur::binnenLampAan(bool force)
{
    if (!noodKnipper || force) {
        sendMsg("binnenLampAan\r");

        ledBinnen = 1;
    }
}

void Deur::binnenLampUit(bool force)
{
    if (!noodKnipper || force) {
        sendMsg("binnenLampUit\r");

        ledBinnen = 0;
    }
}

void Deur::noodKnipperAan()
{
    noodKnipper = 1;
    knipperTimer = std::clock();
}

void Deur::noodKnipperUit()
{
    noodKnipper = 0;
    knipperTimer = 0;
}

bool Deur::updateStatus()
{
    char buffer[256] = {0};

    sendMsg("getStatus\r");

    /* send message and check if client is still connected */
    if (!recvMsg(buffer)) {
        std::cout << "Deur disconnected from socket: " << sock << std::endl;
        return false;
    }

    /* try and catch json parse exceptions */
    try {
        auto j_deur = json::parse(buffer);

        knopBinnen = j_deur.at("binnenKnop");
        knopBuiten = j_deur.at("buitenKnop");
    }
    catch (json::exception& e) {
        std::cout << "Exception error at Deur: " << e.what() << std::endl;
    }
    return true;
}

json Deur::getStatus()
{
    json deurData;
    deurData["Deur"] = {{"Deur", state ? "open" : "dicht"}, {"Binnenknop", knopBinnen}, {"Buitenknop", knopBuiten}, {"Binnenled", ledBinnen}, {"Buitenled", ledBuiten}};

    return deurData;
}
