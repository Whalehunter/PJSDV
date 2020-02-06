#include "Bed.hpp"

using json = nlohmann::json;

Bed::Bed(int sockId, Appartement* ap) : Device(sockId, ap), state(UIT)
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
    /* close connection before exiting the thread */
    close(socket.getId());
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
    int socketId = socket.getId();
    socket.sendBuffer("getStatus\r");
    if (!socket.receiveBuffer(buffer)) {
        std::cout << "Bed disconnected from socket: " << socketId << std::endl;
        close(socketId);
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
	std::cout << "Exception error at Bed: " << e.what() << std::endl;
    }
    return true;
}

void Bed::setLed(bool x)
{
    /* lock the mutex to protect critical data */
    const std::lock_guard<std::mutex>lock (led_mutex);
    if (x) {
	socket.sendBuffer("lampAan\r");
	state = AAN;
    } else {
	socket.sendBuffer("lampUit\r");
	state = UIT;
    }
    /* mutex lock is automatically released upon leaving the function */
}
