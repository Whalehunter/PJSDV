#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"
#include "../Componenten/RGB.hpp"

class Muur: public Device
{
    static const int LAMPEN = 3; // Het aantal aanwezige LEDs

    int ldr;                    // Lichtsensor waarde
    int pot;                    // Potentiometer waarde
    RGBLed lampen[LAMPEN];      // Lijst van LEDs
    int raam;                   // Raam status waarde

    bool disco;                 // Discostand aan/uit
    clock_t discoTimer;         // Discotimer (halve seconde schakelen per licht)

public:
    Muur(int, Appartement*);    // Constructor
    ~Muur();                    // Destructor
    bool ldrOverride = false;   // Of lichtsensor wordt

    void operator()();          // thread loop
    nlohmann::json getStatus(); // JSON object variabelen output
    bool updateStatus();        // thread loop WEMOS afhandeling
    void ToggleLed(int);        // Lamp aan/uit
    bool isDisco();             // Kijken of discostand aan staat
    void RGBaan();              // Lampen aan
    void RGBuit();              // Lampen uit
    void setDisco(bool);        // Discostand aan/uit
    void LCDdimmen();           // Raam donker
    void LCDdoorlaten();        // Raam doorzichtig
    void setBrightness(bool);   // Helderheid van RGB aanpassen

    std::string arduinoStatus(); // Wemos vertellen wat de status is van de variabelen
};
#endif
