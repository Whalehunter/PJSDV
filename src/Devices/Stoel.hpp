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
        void ledAanUit();
        void trilAanUit();
    private:
        int drukknop{};
        int druksensor{};
        int ledStatus{};
        int trilStatus{};
};

#endif
