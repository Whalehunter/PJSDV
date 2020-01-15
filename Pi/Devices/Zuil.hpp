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

        void operator()();
        void noodAlarmAan();
        void noodAlarmUit();
        void brandAlarmAan();
        void brandAlarmUit();
        void deurBelAan();
        void deurBelUit();
        void zoemerAan();
        void zoemerUit();

        nlohmann::json getStatus();
};

#endif
