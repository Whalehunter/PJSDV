#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"

class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
        bool updateStatus();
        nlohmann::json getStatus();
        void updateWaardes();
        void ToggleLed(int);
        int getDruksensor();
    private:
        enum bedStatus {UIT, AAN};

        Bed::bedStatus state;
        int druksensor;
        int knop;
};

#endif
