#ifndef STOEL_HPP
#define STOEL_HPP

#include "../Device.hpp"
#include "../nlohmann/json.hpp"

class Stoel: public Device
{
    int drukknop{};             // Drukknop status
    int drukSensor{};           // Zit sensor status
    int ledStatus{};            // Lamp status
    int trilStatus{};           // Tristand status

public:
    Stoel(int, Appartement*); // Constructor
    ~Stoel();                 // Destructor
    void operator()();        // Thread loop
    nlohmann::json getStatus(); // JSON object status naar GUI
    void ledAan();              // Lamp aan
    void ledUit();              // Lamp uit
    void trilAan();             // Trilstand aan
    void trilUit();             // Trilstand uit
    void toggleLed();           // Toggle lamp
    void toggleTril();          // Toggle trilstand
};

#endif
