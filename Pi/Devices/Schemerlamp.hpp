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
    std::mutex disco_mutex;	// Beschermt de setDisco functie tegen thread collisions
    std::mutex kleur_mutex;	// Beschermt de setKleur functie
    std::mutex lamp_mutex;	// Beschermt de setLamp functie

public:
    Schemerlamp(int, Appartement*); // Constructor
    ~Schemerlamp();                 // Destructor

    void operator()();
    void setKleur(int,int,int); // Kleur setten voor lamp
    bool isDisco();             // Discostand status ophalen
    void setDisco(bool);        // Discostand aan/uit zetten
    void setLamp(bool);		// Lamp aan/uit zetten

    bool updateStatus();        // WEMOS import status update
    nlohmann::json getStatus(); // JSON object met schemerlamp waarden
};
#endif
