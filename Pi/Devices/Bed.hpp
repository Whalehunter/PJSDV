#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"

class Bed: public Device
{
public:
    Bed(int, Appartement*);     // Constructor
    ~Bed();                     // Destructor

    void operator()();          // aparte thread loop
    bool updateStatus();        // pakt waardes van operator (arduino) om zichzelf te updaten
    nlohmann::json getStatus(); // output json object van eigen waarden
    void ToggleLed(int);        // Lamp togglen
    void ledAan();              // Lamp aanzetten
    void ledUit();              // Lamp uitzetten
    int getDruksensor();        // Druksensor op bed opvragen
private:
    enum bedStatus {UIT, AAN};  // lamp waarden

    Bed::bedStatus state;       // Of de lamp aan of uit is
    int druksensor;             // Of er op het bed gelegen wordt
    int knop;                   // Knop van bed
};

#endif
