#include "Deur.hpp"
#include "Zuil.hpp"

using json = nlohmann::json;

Deur::Deur(int n, Appartement* ap): Device(n, ap), state(DICHT), knopBinnen(0), knopBuiten(0), ledBinnen(0), ledBuiten(0), noodKnipper(0), knipperTimer(0)
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
    int socketId = socket.getId();

    while (1) {
	/* get and store JSON values, break from while if Deur is #gone */
	if (!updateStatus()) break;

	/* state machine, if state = closed && knop == pressed, open door, etc  */
	switch (state) {
	    case OPEN:
		if (knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
		    setDeur("dicht");
		}
		break;
	    case DICHT:
		if (knopBinnen == 2 && knopBinnenPrev != knopBinnen) {
		    setDeur("open");
		}
		break;
	}

	/* operations based on checks, check doorbell */
	if (knopBuiten == 1 && knopBuitenPrev != knopBuiten) {
	    setDeurBel(true);
	    setBuitenLamp(true);
	}
	else if (knopBuiten == 0 && knopBuitenPrev != knopBuiten) {
	    setDeurBel(false);
	}

	/* turn external led off after 30 seconds */
	if (ledBuiten == 1 && compareTime(timer, 30.0)) {
	    setBuitenLamp(false);
	}

	/* if fire alarm, blink internal led (1s interval) */
	if (noodKnipper == 1 && compareTime(knipperTimer, 1.0)) {
	    if (ledBinnen) {
		/* setBinnenLamp: first parameter = on/off, second is to force priority */
		setBinnenLamp(false, true);
		knipperTimer = std::clock();
	    }
	    else if (!ledBinnen) {
		setBinnenLamp(true, true);
		knipperTimer = std::clock();
	    }
	}

	/* store old value in Prev variables */
	knopBinnenPrev = knopBinnen;
	knopBuitenPrev = knopBuiten;
    }
    /* close connection when dropping out of while and before exiting the thread */
    close(socketId);
}

void Deur::setDeur(const char* s)
{
    /* mutual exclusivity in critical section is guaranteed */
    const std::lock_guard<std::mutex> lock (deur_mutex);
    if(strcmp(s, "open") == 0) {
        socket.sendBuffer("deurOpen\r");
        state = OPEN;
    } else {
        socket.sendBuffer("deurDicht\r");
        state = DICHT;
    }
    /* mutex is automatically unlocked upon leaving the scope */
}

void Deur::setDeurBel(bool x)
{
    char cZuil = 'f';
    /* check if device exists, then call its function */
    /* dynamic cast to reach child functions */
    if (a->devices.count(cZuil))
        dynamic_cast<Zuil *>(a->devices.find(cZuil)->second)->setDeurBel(x);
}

void Deur::setBuitenLamp(bool x)
{
    /* mutual exclusivity in critical section is guaranteed */
    const std::lock_guard<std::mutex>lock (buitenlamp_mutex);
    if (x) {
	/* buitenLamp pff and reset timer */
        socket.sendBuffer("buitenLampAan\r");
        timer = std::clock();
    } else {
        socket.sendBuffer("buitenLampUit\r");
        timer = 0;
    }
    ledBuiten = x;
}

void Deur::setBinnenLamp(bool x, bool force)
{
    /* light can only be accessed if the fire alarm isn't on, fire alarm uses force */
    if (!noodKnipper || force) {
	/* mutex lock to guard critical data */
        const std::lock_guard<std::mutex>lock (binnenlamp_mutex);
        if (x) {
            socket.sendBuffer("binnenLampAan\r");
        } else {
            socket.sendBuffer("binnenLampUit\r");
        }
        ledBinnen = x;
    }
}

void Deur::setNoodKnipper(bool x)
{
    /* mutex lock to protect critcal data */
    const std::lock_guard<std::mutex>lock (noodknipper_mutex);
    if (x) {
        knipperTimer = std::clock();
    } else {
        knipperTimer = 0;
    }
    noodKnipper = x;
}

bool Deur::updateStatus()
{
    char buffer[256] = {0};
    int socketId = socket.getId();
    socket.sendBuffer("getStatus\r");

    /* send message and check if client is still connected */
    if (!socket.receiveBuffer(buffer)) {
        std::cout << "Deur disconnected from socket: " << socketId << std::endl;
        return false;
    }

    /* try and catch json parse exceptions */
    try {
        auto j_deur = json::parse(buffer);

        /* store json values */
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
    /* store class data in json object and return it */
    json deurData;
    deurData["Deur"] = {{"Deur", state ? "open" : "dicht"}, {"Binnenknop", knopBinnen}, {"Buitenknop", knopBuiten}, {"Binnenled", ledBinnen}, {"Buitenled", ledBuiten}};

    return deurData;
}
