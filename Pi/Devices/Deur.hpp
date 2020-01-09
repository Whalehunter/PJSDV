#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Deur: public Device
{
    private:
        enum deurStatus {DICHT, OPEN, OPSLOT};

        Deur::deurStatus state;
        std::clock_t timer;

        int knopBinnen;
        int knopBuiten;
        int ledBinnen;
        int ledBuiten;
    public:
        Deur(int, Appartement*);
        ~Deur();

        void operator()();
        void openDeur();
        void sluitDeur();
        void deurBel();
        void buitenLampAan();
        void buitenLampUit();
        int getLedStatus();

        nlohmann::json getStatus();
};

#endif