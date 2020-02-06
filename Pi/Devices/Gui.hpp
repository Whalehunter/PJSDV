#ifndef GUI_HPP
#define GUI_HPP

#include "../SocketConnection.hpp"
#include <map>

class Device;
class Gui
{
    std::map<char, Device*>* devices;
    SocketConnection socket;

public:
    Gui(int, std::map<char, Device*>);     // Constructor
    ~Gui();                     // Destructor

    void operator()();          // Thread loop
};

#endif
