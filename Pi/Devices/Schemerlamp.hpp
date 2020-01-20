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
    void updateDiscoColor();

    public:
    Schemerlamp(int, Appartement*);
    ~Schemerlamp();

    void operator()();
    void setKleur(int,int,int); // Kleur setten voor lamp
    bool isDisco();             // Discostand status ophalen
    void setDisco(bool);        // Discostand aan/uit zetten
    int getDiscoKleur(std::string); //
    void uit();
    void aan();

    bool updateStatus();
    nlohmann::json getStatus();
};
#endif
