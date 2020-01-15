#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

//#include "Device.hpp"
//#include "Devices/Gui.hpp"
//#include "nlohmann/json.hpp"
//#include <string>
//#include <iostream>
#include <thread>
#include <map>
//#include <string>

class Device;
class Appartement
{
public:
    Appartement();
    ~Appartement();

    std::map<char, Device*> devices;
    Device* deur{};
    Device* gui{};
    Device* bed{};
    Device* stoel{};
    Device* zuil{};
    Device* schemerlamp{};
    Device* muur{};
    Device* koelkast{};

    void createDevice(int, char);
};

#endif
