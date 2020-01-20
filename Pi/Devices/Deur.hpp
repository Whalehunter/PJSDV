#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Deur: public Device
{
private:
    enum deurStatus {DICHT, OPEN}; // status van de deur
    Deur::deurStatus state;        // ^
    int knopBinnen;                // Of de binnenknop ingedrukt is of niet
    int knopBuiten;                // Zelfde, maar voor buitenknop
    int ledBinnen;                 // Of de binnenled aan staat
    int ledBuiten;                 // Zelfde, maar voor buiten lamp
    int noodKnipper;               // Of de binnenlamp in de noodstand moet knipperen
    std::clock_t timer;            // Buitenlamp aan timer
    std::clock_t knipperTimer;     // Binnenlamp knipper timer
public:
    Deur(int, Appartement*); // Constructor
    ~Deur();                 // Destructor

    void operator()();      // Thread loop
    void openDeur();        // Deur openen
    void sluitDeur();       // Deur sluiten
    void deurBelAan();      // Deurbel aan
    void deurBelUit();      // Deurbel uit
    void buitenLampAan();   // Buitenlamp aan
    void buitenLampUit();   // Buitenlamp uit
    void binnenLampAan(bool force = false); // Binnenlamp aan
    void binnenLampUit(bool force = false); // Binnenlamp uit
    void noodKnipperAan();                  // Nood knipperlicht aan
    void noodKnipperUit();                  // Nood knipperlicht uit

    bool updateStatus();    // Thread loop data verwerker (vanuit WEMOS)
    nlohmann::json getStatus(); // JSON object met de deur waarden voor GUI
};

#endif
