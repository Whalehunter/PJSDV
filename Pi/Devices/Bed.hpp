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
    void setLed(bool);        // led on/off
private:
    enum bedStatus {UIT, AAN};  // lamp waarden

    std::mutex led_mutex;	// beschermt de setLed functie tegen thread collision
    Bed::bedStatus state;       // Of de lamp aan of uit is
};

#endif
