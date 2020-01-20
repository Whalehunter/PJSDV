#ifndef GUI_HPP
#define GUI_HPP

#include "../Device.hpp"

class Gui: public Device
{
public:
    Gui(int, Appartement*);     // Constructor
    ~Gui();                     // Destructor

    void operator()();          // Thread loop
    nlohmann::json getStatus(); // Gedwongen implementatie (het is een "Device")
};

#endif
