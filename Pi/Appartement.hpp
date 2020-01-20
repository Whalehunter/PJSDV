#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include <thread>
#include <map>

class Device;
class Appartement
{
public:
    Appartement();
    ~Appartement();

    std::map<char, Device*> devices; // map met device pointers en ID
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
