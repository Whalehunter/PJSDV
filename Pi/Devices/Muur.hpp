#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"

class Muur: public Device
{
    public:
        Muur(int, Appartement*);
        ~Muur();

        void operator()();
        nlohmann::json getStatus();
        bool updateStatus();
        void ToggleLed(int);

        void RGBdimmen();
        void RGBaan();
        void RGBuit();
        void RGBdisco();
        void LCDdimmen();
        void LCDdoorlaten();
    private:
        int ldr;
        int pot;
};
#endif
