#ifndef SCHEMERLAMP_HPP
#define SCHEMERLAMP_HPP

#include "../Device.hpp"
#include "../nlohmann/json.hpp"
#include "../Componenten/RGB.hpp"

class Schemerlamp: public Device
{
    int beweging;
    RGBLed lamp;
    bool disco;
    clock_t discoTimer;
    clock_t activityTimer;
    std::string currentDiscoColor = "rood";
    void updateDiscoColor();

    public:
    Schemerlamp(int, Appartement*);
    ~Schemerlamp();

    void operator()();
    void setLamp(bool);         // aan of uit bool (true = aan)
    void setLamp(std::string);  // aan of uit string
    bool getLamp();
    void setKleur(int,int,int); // Kleur
    bool isDisco();
    void setDisco(bool);
    int getDiscoKleur(std::string);
    void uit();
    void aan();

    bool updateStatus();
    nlohmann::json getStatus();
};
#endif
