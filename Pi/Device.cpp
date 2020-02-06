#include "Device.hpp"

// Constructor
//
// Param n: socket ID
// Param ap: Appartement class pointer
//
// Initialiseert standaard waarden van knopValue,
// sensorValue en de socket verbinding
Device::Device(int n, Appartement* ap): socket(SocketConnection(n, 255)), a(ap), knopValue(0), sensorValue(0), timer(0)
{}

Device::~Device()
{}

bool Device::compareTime(std::clock_t timer, double timeLimit)
{
    /* compare timer with set time, return true/false depending on whether
     * it has passed the configured time limit
     */
    return (((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= timeLimit);
}

int Device::getSensor()
{
    return sensorValue;
}

int Device::getKnop()
{
    return knopValue;
}
