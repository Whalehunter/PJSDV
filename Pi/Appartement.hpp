#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include <thread>
#include <map>
#include <vector>
#include "Devices/Gui.hpp"
#include <mutex>

// forward decleration
class Device;
class Appartement
{
public:
    Appartement();
    ~Appartement();

    std::map<char, Device*> devices;
    std::vector<Gui*> guis;

    bool createDevice(int, char); // maakt objecten aan voor de connecting devices
};

#endif
