#include "Device.hpp"

// Constructor
//
// Param n: socket ID
// Param ap: Appartement class pointer
//
// Initialiseert standaard waarden van knopValue,
// sensorValue en de socket verbinding
Device::Device(int n, Appartement* ap): socket(SocketConnection(n, 255)), a(ap), knopValue(0), sensorValue(0)
{}

Device::~Device()
{}

int Device::getSensor()
{
    return sensorValue;
}

int Device::getKnop()
{
    return knopValue;
}
