#include "Stoel.hpp"

using json = nlohmann::json;

Stoel::Stoel(int n, Appartement* ap): Device(n, ap)//Stoel Aanmaken
{
    std::cout << "Stoel aangemaakt" << std::endl;
}

Stoel::~Stoel()//Stoel vernietigen
{}

void Stoel::operator()()//Operrerererererererereeratie functie van stoel
{
    char buffer[256] = {0};
    int knopPrev = 0;
    int socketId = socket.getId();

    while (1) {
        socket.sendBuffer("getStatus\r");//Stuur getStatus naar device

        if (!socket.receiveBuffer(buffer)) {
            std::cout << "Stoel disconnected from socket: " << socketId << std::endl;
            close(socketId);
            return;
        }

        try {
            auto j_stoel = json::parse(buffer);
            knopValue = j_stoel.at("drukknop");
            sensorValue = j_stoel.at("drukSensor");
        }
        catch (json::exception& e) {
            std::cout << e.what() << std::endl;
        }

        if (knopValue && !knopPrev) {
            toggleLed();
        }

        if (!sensorValue && trilStatus) {
            setTril(false);
        } else if (knopValue && !knopPrev && sensorValue) {
            toggleTril();
        }

        knopPrev = knopValue;
    }
    close(socketId);
    std::cout << "Connection closed on socket " << socket.getId() << std::endl;
}

json Stoel::getStatus() //Get Status voor de GUI
{
    json stoel;
    stoel["Stoel"] = {{"Knop", knopValue}, {"Lamp", ledStatus}, {"Massage", trilStatus}, {"Drukplaat", sensorValue}};
    return stoel;
}

void Stoel::toggleLed() // Leeslamp toggle
{
    setLed(!ledStatus);
}


void Stoel::setLed(bool x) // Leeslamp aan/uit
{
    const std::lock_guard<std::mutex> lock (led_mutex);
    if (x) {
	socket.sendBuffer("ledAan\r");
    } else {
	socket.sendBuffer("ledUit\r");
    }
    ledStatus = x;
}

void Stoel::setTril(bool x) // Tril modus aan/uit
{
    const std::lock_guard<std::mutex> lock (tril_mutex);
    if (x) {
	socket.sendBuffer("trilAan\r");
    } else {
	socket.sendBuffer("trilUit\r");
    }
    trilStatus = x;
}

void Stoel::toggleTril() // Toggle tril modus
{
    setTril(!trilStatus);
}
