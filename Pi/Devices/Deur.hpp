#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"
//#include "../nlohmann/json.hpp"

class Deur: public Device
{
    private:
        enum deurStatus {DICHT, OPEN, OPSLOT};

        Deur::deurStatus state;
        int knopBinnen;
        int knopBuiten;
        int ledBinnen;
        int ledBuiten;
    public:
        Deur(int, Appartement*);
        ~Deur();

       // enum status {dicht, open, opSlot};

        void operator()();
        void openDeur();
        void sluitDeur();
        void deurBel();
        int getStatus();
        int getLedStatus();

        nlohmann::json getDeurStatus();
};

#endif
