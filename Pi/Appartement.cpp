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

Appartement::Appartement(): deur(0), gui(0), bed(0), stoel(0), zuil(0), muur(0), koelkast(0)
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
    if (id == 'd') {
        if(deur != 0) delete deur;
        deur = new Deur(sock, this);
        ob = deur;
    }
    else if (id == 'f') {
        if(zuil != 0) delete zuil;
        zuil = new Zuil(sock, this);
        ob = zuil;
    }
    else if (id == 's') {
        if(schemerlamp != 0) delete schemerlamp;
        schemerlamp = new Schemerlamp(sock, this);
        ob = schemerlamp;
    }
    else if (id == 'x') {
       // if(gui != 0) delete gui;
       // gui = new Gui(sock, this);
        ob = new Gui(sock, this);
    }
    else if (id == 'y') {
        if(bed != 0) delete bed;
        bed = new Bed(sock, this);
        ob = bed;
    }
    else if (id == 'z') {
        if(stoel != 0) delete stoel;
        stoel = new Stoel(sock, this);
        ob = stoel;
    }
    else if (id == 'm') {
        if(muur != 0) delete muur;
        muur = new Muur(sock, this);
        ob = muur;
    }
    else if (id == 'k') {
        if(koelkast != 0) delete koelkast;
        koelkast = new Koelkast(sock, this);
        ob = koelkast;
    }

    if (ob != NULL) {
        devices.insert(std::pair<char, Device*>(id, ob));
        std::thread obThread(&Device::operator(), ob);
        obThread.detach();
    }
}
