#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include "Device.hpp"
#include <map>

class Appartement
{
    static Appartement* instance;
    std::map<std::string, Device*> deviceNamePair;

    Appartement();

public:

    static Appartement* getInstance();
    void registerDevice(std::string, int);
};

#endif
