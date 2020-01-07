#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include "Device.hpp"
#include <map>
#include <string>

class Appartement
{
    static Appartement* instance;
    std::map<std::string, Device*> deviceNamePairs;
    Appartement();
    const Device* getDevice(std::string name);
    std::string capitalize(const std::string name);
public:
    ~Appartement();
    const Device * deviceExists(std::string name);
    static Appartement* getInstance();
    void registerDevice(std::string, int);
};

#endif
