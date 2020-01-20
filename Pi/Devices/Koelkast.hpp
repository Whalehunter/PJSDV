#ifndef DEVICES_KOELKAST_HPP_
#define DEVICES_KOELKAST_HPP_

#include "../Device.hpp"
#include <iostream>
#include <iomanip>

class Koelkast: public Device {
    std::clock_t openTimer;

    int koelkastDeur{};         // Deur open/dicht
    int fan{};                  // Waaier aan/uit
    float NTC1{};               // Temperatuursensor buiten
    float NTC2{};               // Temperatuursensor binnen
    int koelAlarm{};            // Aan als deur te lang open staat
    float tempOut{};            // Omgerekende temperatuur buiten
    float tempIn{};             // Omgerekende temperatuur binnen
    int koelelement{};          // Peltier element aan/uit

public:
    Koelkast(int, Appartement*); // Constructor
    ~Koelkast();                 // Destructor
    nlohmann::json getStatus();  // JSON object van variabele waarden
    void operator()();           // thread loop
    void disableKoelAlarm();     // Koelalarm uitzetten
    void fanAan();               // Waaier aan
    void fanUit();               // Waaier uit
    void peltierAan();           // Koelelement aan
    void peltierUit();           // Koelelement uit
    float calculateCelsius(float i); // Peltier analoge waarden omzetten naar celcius
};


#endif /* DEVICES_KOELKAST_HPP_ */
