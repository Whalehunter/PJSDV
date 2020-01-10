#ifndef MUUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Muur: public Device
{
    private:
        enum muurStatus {LCDDIMMEN, LCDDOORLATEN};

        Muur::muurStatus state;

        int ldr;
        int pot;
    public:
        Muur(int, Appartement*);
        ~Muur();

        void operator()();
        nlohmann::json getStatus();
        void LCDdimmen();
        void LCDdoorlaten();
        void RGBdimmen();
        void RGBaan();
        void RGBuit();
        void RGBdisco();
};
#endif
