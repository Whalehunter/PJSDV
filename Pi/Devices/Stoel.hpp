#ifndef STOEL_HPP
#define STOEL_HPP

#include "../Device.hpp"
#include "../nlohmann/json.hpp"

class Stoel: public Device
{
    public:
        Stoel(int, Appartement*);
        ~Stoel();
        void operator()();
        nlohmann::json getStatus();
        bool updateStatus();
        void ledAan();
        void ledUit();
        void trilAan();
        void trilUit();
    private:
        int drukknop{};
        int drukSensor{};
        int ledStatus{};
        int trilStatus{};
};

#endif
