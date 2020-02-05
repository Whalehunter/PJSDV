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
    delete deur;
    delete gui;
    delete stoel;
    delete bed;
    delete zuil;
    delete schemerlamp;
    delete muur;
    delete koelkast;
}

void Appartement::createDevice(int sock, char id)
{
    Device * ob = NULL;
    switch(id){

    /* create objects based on handshake ID, delete old pointer if not null */
    case 'd':
        if(deur != 0) delete deur;
        deur = new Deur(sock, this);
        ob = deur;
        break;

    case 'f':
        if(zuil != 0) delete zuil;
        zuil = new Zuil(sock, this);
        ob = zuil;
        break;

    case 'm':
        if(muur != 0) delete muur;
        muur = new Muur(sock, this);
        ob = muur;
        break;

    case 'k':
        if(koelkast != 0) delete koelkast;
        koelkast = new Koelkast(sock, this);
        ob = koelkast;
        break;

    case 's':
        if(schemerlamp != 0) delete schemerlamp;
        schemerlamp = new Schemerlamp(sock, this);
        ob = schemerlamp;
        break;

    case 'x':
        ob = new Gui(sock, this);
        break;

    case 'y':
        if(bed != 0) delete bed;
        bed = new Bed(sock, this);
        ob = bed;
        break;

    case 'z':
        if(stoel != 0) delete stoel;
        stoel = new Stoel(sock, this);
        ob = stoel;
        break;
    }

    if (ob != NULL) {
	/* insert device into devices map, id(char) + pointer to object */
        devices.insert(std::pair<char, Device*>(id, ob));
	/* pass pointer to function and object to thread */
        std::thread obThread(&Device::operator(), ob);
	/* detach so that it may operate independently of other threads */
        obThread.detach();
    }
}
