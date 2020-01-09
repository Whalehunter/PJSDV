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
        void activeerBewegingssensor();
        void deactiveersBewegingssensor();
        void ToggleLed(int);
    private:
        enum bedStatus {LICHT, DONKER};

        Bed::bedStatus state;
        int druksensor;
        int knop;
};

#endif
