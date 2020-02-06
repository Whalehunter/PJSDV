#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Deur: public Device
{
private:
    enum deurStatus {DICHT, OPEN}; 	// status van de deur
    Deur::deurStatus state;        	// ^
    int knopBinnen;                	// Of de binnenknop ingedrukt is of niet
    int knopBuiten;                	// Zelfde, maar voor buitenknop
    bool ledBinnen;                 	// Of de binnenled aan staat
    bool ledBuiten;                 	// Zelfde, maar voor buiten lamp
    bool noodKnipper;               	// Of de binnenlamp in de noodstand moet knipperen

    std::mutex deur_mutex;		// Beschermt setDeur() 
    std::mutex deurbel_mutex;		// Beschermt setDeurBel()
    std::mutex buitenlamp_mutex;	// Beschermt setBuitenLamp()
    std::mutex binnenlamp_mutex;	// Beschermt setBinnenLamp()
    std::mutex noodknipper_mutex;	// Beschermt setNoodKnipper()

    std::clock_t knipperTimer;     	// Binnenlamp knipper timer
public:
    Deur(int, Appartement*); 		// Constructor
    ~Deur();                 		// Destructor

    void operator()();      		// Thread loop
    void setDeur(const char*); 		// Deur open/dicht
    void setDeurBel(bool);  		// Deurbel
    void setBuitenLamp(bool); 		// Buitenlamp aan/uit
    void setBinnenLamp(bool, bool force = false); // Binnenlamp aan/uit, force wordt gebruikt om prioriteit aan het brandalarm te geven
    void setNoodKnipper(bool); 		// Knipperlicht bij brand aan/uit

    bool updateStatus();    		// Thread loop data verwerker (vanuit WEMOS)
    nlohmann::json getStatus(); 	// JSON object met de deur waarden voor GUI
};

#endif
