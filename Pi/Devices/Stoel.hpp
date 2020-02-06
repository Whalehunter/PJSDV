#ifndef STOEL_HPP
#define STOEL_HPP

#include "../Device.hpp"
#include "../nlohmann/json.hpp"

class Stoel: public Device
{
    int ledStatus{};            // Lamp status
    int trilStatus{};           // Tristand status

    std::mutex tril_mutex;	// Beschermt de trilfunctie
    std::mutex led_mutex;	// Beschermt de setLed functie

public:
    Stoel(int, Appartement*); // Constructor
    ~Stoel();                 // Destructor
    void operator()();        // Thread loop
    nlohmann::json getStatus(); // JSON object status naar GUI
    void setLed(bool);		// Led aan/uit
    void toggleLed();           // Toggle led
    void setTril(bool);		// Trilfunctie aan/uit
    void toggleTril();		// toggle trilfunctie
};

#endif
