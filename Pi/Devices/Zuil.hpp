#ifndef ZUIL_HPP
#define ZUIL_HPP

#include "../Device.hpp"

class Zuil: public Device
{
    private:
        bool nood;              // Nood status
        bool brand;             // Brand status
        bool zoemer;            // Zoemer status

	std::mutex deurbel_mutex;	// beschermt de deurbel functie zodat maar 1 thread op een gegeven moment de variables kan aanpassen
	std::mutex noodalarm_mutex;	// beschermt de noodalarm functie
	std::mutex zoemer_mutex;	// beschermt de zoemer functie
	std::mutex brandalarm_mutex;	// beschermt de brandalarm functie
    public:
        Zuil(int, Appartement*);
        ~Zuil();

        void operator()();    		// thread loop
	void setNoodAlarm(bool); 	// noodalarm aan/uit
	void setBrandAlarm(bool); 	// brandalarm aan/uit
        void setDeurBel(bool);    	// deurbel uit
	void setZoemer(bool); 		// zoemer aan/uit

        nlohmann::json getStatus(); 	// json object met alle variables van zuil
};

#endif
