#include "Zuil.hpp"
#include "Deur.hpp"

using json = nlohmann::json;

Zuil::Zuil(int n, Appartement* ap): Device(n, ap), nood(0), brand(0), zoemer(0)
{
    std::cout << "Zuil aangemaakt" << std::endl;
}

Zuil::~Zuil()
{}

void Zuil::operator()()
{
    char buffer[256] = {0};

    while (1) {
	sendMsg("getStatus\r");

	/* receive msg and check if device is still connected to socket */
	if(!recvMsg(buffer)) {
	    std::cout << "Zuil disconnected from socket: " << sock << std::endl;
	    close(sock);
	    return;
	}

	/* try and catch json exception errors */
	try {
	    auto j_zuil = json::parse(buffer);

	    /* store json values */
	    knopValue = j_zuil.at("knopValue");
	    sensorValue = j_zuil.at("sensorValue");
	}
	catch (json::exception& e) {
	    std::cout << "Exception error at Zuil: " << e.what() << std::endl;
	}

	if (getKnop()) {
	    setNoodAlarm(true);
	}
	if (getSensor() >= 920) {
	    setBrandAlarm(true);
	}
    }
    /* close connection before exiting thread */
    close(sock);
}

void Zuil::setNoodAlarm(bool x)
{
    /* lock the mutex and turn nood alarm on/off, only allow alarm off if there is no fire alarm */
    const std::lock_guard<std::mutex>lock (noodalarm_mutex);
    if (x) {
	setZoemer(x);
    } else {
	if(!brand) {
	    setZoemer(x);
	}
    }
    nood = x;
    /* lock is automatically released upon leaving the function */
}

void Zuil::setBrandAlarm(bool x)
{
    /* lock the mutex, fire alarm on/off */
    const std::lock_guard<std::mutex>lock (brandalarm_mutex);
    if (!nood) {
	setZoemer(x);
    }
    brand = x;
    /* check if deur exists, then call its setNoodKnipper function with true/false */
    if (a->devices.count('d')) {
	dynamic_cast<Deur *>(a->devices.find('d')->second)->setNoodKnipper(x);
    }
    /* lock is automatically released upon leaving the function */
}

void Zuil::setDeurBel(bool x)
{
    /* lock the mutex, only alter zoemer value if no emergency or fire alarm */
    const std::lock_guard<std::mutex>lock (deurbel_mutex);
    if (!nood && !brand) {
	setZoemer(x);
    }
    /* lock is automatically released upon leaving the function */
}

void Zuil::setZoemer(bool x)
{
    /* lock the mutex, zoemer on/off */
    const std::lock_guard<std::mutex>lock (zoemer_mutex);
    if (x) {
	sendMsg("zoemerAan\r");
    } else {
	sendMsg("zoemerUit\r");
    }
    zoemer = x;
    /* lock is automatically released upon leaving the function */
}

json Zuil::getStatus()
{
    /* MUTEX LOCK REQ */
    /* store class variables in json object and return it */
    json zuilData;
    zuilData["Zuil"] = {{"Zoemer", zoemer}, {"Noodalarm", nood}, {"Brandalarm", brand}, {"Gasmeter", sensorValue}, {"Knop", knopValue}};

    return zuilData;
}
