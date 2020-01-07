#include "Schemerlamp.hpp"

Schemerlamp::Schemerlamp(int socketId): Device(socketId)
{
    this->name = "Schemerlamp";
    std::cout << "Schemerlamp aangemaakt" << std::endl;
}
