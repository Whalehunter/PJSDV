#ifndef ZUIL_HPP
#define ZUIL_HPP

#include "../Device.hpp"

class Zuil: public Device
{
    private:
        int nood;
        int brand;
        int zoemer;

        clock_t timer;
    public:
        Zuil(int, Appartement*);
        ~Zuil();

        void operator()();    // thread loop
        void noodAlarmAan();  // noodalarm aan
        void noodAlarmUit();  // noodalarm uit
        void brandAlarmAan(); // brandalarm aan
        void brandAlarmUit(); // brandalarm uit
        void deurBelAan();    // deurbel aan
        void deurBelUit();    // deurbel uit
        void zoemerAan();     // zoemer aan
        void zoemerUit();     // zoemer uit

        nlohmann::json getStatus(); // json object met alle variables van zuil
};

#endif
