#ifndef DEVICES_KOELKAST_HPP_
#define DEVICES_KOELKAST_HPP_

#include "../Device.hpp"
#include <iostream>
#include <iomanip>

class Koelkast: public Device {
    int koelkastDeur{};         // Deur open/dicht
    bool fan{false};                  // Waaier aan/uit
    float NTC1{};               // Temperatuursensor buiten
    float NTC2{};               // Temperatuursensor binnen
    int koelAlarm{};            // Aan als deur te lang open staat
    float tempOut{};            // Omgerekende temperatuur buiten
    float tempIn{};             // Omgerekende temperatuur binnen
    bool koelelement{false};          // Peltier element aan/uit

    std::mutex fan_mutex;	// Beschermt de fan functie tegen thread collision
    std::mutex peltier_mutex;	// Beschermt de peltier functie
public:
    Koelkast(int, Appartement*); // Constructor
    ~Koelkast();                 // Destructor
    nlohmann::json getStatus();  // JSON object van variabele waarden
    void operator()();           // thread loop
    void disableKoelAlarm();     // Koelalarm uitzetten
    void setFan(bool);		// Fan aan/uit
    void setPeltier(bool);	// Peltier aan/uit
    float calculateCelsius(float i); // Peltier analoge waarden omzetten naar celcius
};


#endif /* DEVICES_KOELKAST_HPP_ */
