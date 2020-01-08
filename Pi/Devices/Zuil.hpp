#ifndef ZUIL_HPP
#define ZUIL_HPP

#include "../Device.hpp"

class Zuil: public Device
{
    private:
        int zoemer;
        int brand;
        int nood;
    public:
        Zuil(int, Appartement*);
        ~Zuil();

        void operator()();
        void noodAlarm(int);
        void brandAlarm(int);
        void zoemerAan();
        void zoemerUit();

        nlohmann::json getStatus();
};

#endif
