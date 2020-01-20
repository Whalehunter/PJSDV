#ifndef SCHEMERLAMP_HPP
#define SCHEMERLAMP_HPP

#include "../Device.hpp"
#include "../nlohmann/json.hpp"
#include "../Componenten/RGB.hpp"

class Schemerlamp: public Device
{
    int beweging;               // Beweging detectie waarde
    RGBLed lamp;                // Lamp waarden
    bool disco;                 // Disco status
    clock_t discoTimer;         // Disco timer waarde
    clock_t activityTimer;      // Activity timer waarde

public:
    Schemerlamp(int, Appartement*); // Constructor
    ~Schemerlamp();                 // Destructor

    void operator()();
    void setKleur(int,int,int); // Kleur setten voor lamp
    bool isDisco();             // Discostand status ophalen
    void setDisco(bool);        // Discostand aan/uit zetten
    void uit();                 // Lamp uit
    void aan();                 // Lamp aan

    bool updateStatus();        // WEMOS import status update
    nlohmann::json getStatus(); // JSON object met schemerlamp waarden
};
#endif
