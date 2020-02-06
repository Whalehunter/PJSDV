#include "Bed.hpp"

using json = nlohmann::json;

Bed::Bed(int sockId, Appartement* ap) : Device(sockId, ap)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{}

void Bed::operator()()
{
    int knopPrev = 0;

    while (1) {
        if (!updateStatus()) break;

        /*State Machine*/
        switch (state) {
        case AAN:
            if (knop && !knopPrev) {
                ledUit();
            }
            break;
        case UIT:
            if (knop && !knopPrev) {
                ledAan();
            }
        }
        knopPrev = knop;
    }
}

nlohmann::json Bed::getStatus()
{
    json bedData;
    bedData["Bed"] = {{"Lamp", state}, {"knop", knop}, {"drukSensor", druksensor ? "Bezet" : "Beschikbaar"}};
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

    try {
        auto j_bed = json::parse(buffer);

        knop = j_bed.at("knop");
        druksensor = j_bed.at("druksensor");
    }
    catch (json::exception& e) {
        std::cout << "Parsing error at Bed on socket " << socketId << std::endl;
    }
    return true;
}

void Bed::ToggleLed(int i)
{
    if (i) {
        ledAan();
    } else {
        ledUit();
    }
}

void Bed::ledAan()
{
    socket.sendBuffer("lampAan\r");
    state = AAN;
}

void Bed::ledUit()
{
    socket.sendBuffer("lampUit\r");
    state = UIT;
}

int Bed::getDruksensor()
{
    return druksensor;
}
