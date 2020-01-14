#include "Muur.hpp"

Muur::Muur(int socketId): Device(socketId)
{
    this->name = "Muur";
    std::cout << "Muur aangemaakt" << std::endl;
}
