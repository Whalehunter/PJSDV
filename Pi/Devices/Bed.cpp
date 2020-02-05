#include "Bed.hpp"

using json = nlohmann::json;

Bed::Bed(int n, Appartement* ap): Device(n, ap), state(UIT)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{}

void Bed::operator()()
{
    int knopPrev = 0;

    while (1) {
	/* check if device is still connected, else break from while */
	if (!updateStatus()) break;

	/* state machine */
	switch (state) {
	    case AAN:
		/* if button is pressed and led = on -> led off */
		if (knopValue && !knopPrev) {
		    setLed(false);
		}
		break;
	    case UIT:
		/* if button is pressed and led = off -> led on */
		if (knopValue && !knopPrev) {
		    setLed(true);
		}
	}
	/* store current button value in prev for debouncing */
	knopPrev = knopValue;
    }
}

nlohmann::json Bed::getStatus()
{
    /* store data in json object and return json */
    json bedData;
    bedData["Bed"] = {{"Lamp", state}, {"knop", knopValue}, {"drukSensor", sensorValue ? "Bezet" : "Beschikbaar"}};
    return bedData;
}

bool Bed::updateStatus()
{
    char buffer[256] = {0};
    sendMsg("getStatus\r");

    /* if recv returns < 0 then device has disconnected */
    if (!recvMsg(buffer)) {
	std::cout << "Bed disconnected from socket: " << sock << std::endl;
	close(sock);
	return false;
    }

    /* try and catch exceptions */
    try {
	auto j_bed = json::parse(buffer);

	/* store values */
	knopValue = j_bed.at("knop");
	sensorValue = j_bed.at("druksensor");
    }
    catch (json::exception& e) {
	std::cout << "Parsing error at Bed on socket " << sock << std::endl;
    }
    return true;
}

void Bed::setLed(bool x)
{
    /* lock the mutex to protect critical data */
    const std::lock_guard<std::mutex>lock (led_mutex);
    if (x) {
	sendMsg("lampAan\r");
	state = AAN;
    } else {
	sendMsg("lampUit\r");
	state = UIT;
    }
    /* mutex lock is automatically released upon leaving the function */
}
