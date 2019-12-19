#ifndef GUI_HPP
#define GUI_HPP

#include "../Device.hpp"

class Gui: public Device
{
    public:
        Gui(int, Appartement*);
        ~Gui();

        void operator()();
        void getStatus();
};

#endif
