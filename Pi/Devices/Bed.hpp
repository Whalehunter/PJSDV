#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"


class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
        nlohmann::json getStatus();
        void updateWaardes();
        void ToggleLed(int);
    private:
        enum bedStatus {UIT, AAN};

        Bed::bedStatus state;
        int druksensor;
        int knop;
};

#endif
