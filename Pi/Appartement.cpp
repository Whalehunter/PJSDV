#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Deur.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include "Devices/Zuil.hpp"
#include "Devices/Schemerlamp.hpp"
#include "Devices/Muur.hpp"
#include "Devices/Koelkast.hpp"

Appartement::Appartement()
{}

Appartement::~Appartement()
{
    std::map<char, Device*>::iterator deviceIterator = devices.begin();
    while (deviceIterator != devices.end()) {
        delete deviceIterator->second;
    }
}

/**
 * Maak nieuwe device aan op basis van
 *
 * param sock: Socket ID
 * param id:   Identificatie van de device
 *
 * Return Boolean: Is het aanmaken van de device gelukt?
 */
bool Appartement::createDevice(int sock, char id)
{
    Device * ob = NULL;
    switch (id) {
    case 'd': ob = new Deur(sock, this); break;
    case 'f': ob = new Zuil(sock, this); break;
    case 'm': ob = new Muur(sock, this); break;
    case 'k': ob = new Koelkast(sock, this); break;
    case 's': ob = new Schemerlamp(sock, this); break;
    case 'y': ob = new Bed(sock, this); break;
    case 'z': ob = new Stoel(sock, this); break;
    case 'x':
        Gui * gui = new Gui(sock, devices);
        guis.push_back(gui);
        std::thread guiThread(&Gui::operator(), gui);
        guiThread.detach();
        return true;
    }

    if (ob != NULL) {
        /**
         * Delete bestaande devices als een andere socket zich aangeeft als de device
         */
        auto search = devices.find(id);
        if (search != devices.end()) {
            delete search->second;
            devices.erase(search);
        }
	/* insert device into devices map, id(char) + pointer to object */
        devices.insert(std::pair<char, Device*>(id, ob));
	/* pass pointer to function and object to thread */
        std::thread obThread(&Device::operator(), ob);
	/* detach so that it may operate independently of other threads */
        obThread.detach();
        return true;
    }

    return false;
}
