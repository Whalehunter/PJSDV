#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Deur: public Device
{
    private:
        enum deurStatus {DICHT, OPEN, OPSLOT};

        Deur::deurStatus state;


        int knopBinnen;
        int knopBuiten;
        int ledBinnen;
        int ledBuiten;
        int noodKnipper;
        std::clock_t timer;
        std::clock_t knipperTimer;
    public:
        Deur(int, Appartement*);
        ~Deur();

        void operator()();
        void openDeur();
        void sluitDeur();
        void deurBelAan();
        void deurBelUit();
        void buitenLampAan();
        void buitenLampUit();
        void binnenLampAan();
        void binnenLampUit();
        void noodKnipperAan();
        void noodKnipperUit();

        bool updateStatus();
        nlohmann::json getStatus();
};

#endif
