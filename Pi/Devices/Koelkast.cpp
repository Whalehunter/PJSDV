#include "Koelkast.hpp"

Koelkast::Koelkast(int socketId): Device(socketId)
{
    this->name = "Koelkast";
    std::cout << "Koelkast aangemaakt" << std::endl;
}
