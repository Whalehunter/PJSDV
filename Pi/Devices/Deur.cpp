#include "Deur.hpp"

Deur::Deur(int socketId): Device(socketId)
{
    this->name = "Deur";
    std::cout << "Deur aangemaakt" << std::endl;
}
