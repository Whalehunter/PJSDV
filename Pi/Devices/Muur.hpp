#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"
#include "../Componenten/RGB.hpp"

class Muur: public Device
{
    static const int LAMPEN = 3;

    int ldr;
    bool ldrOverride = false;
    int pot;
    RGBLed lampen[LAMPEN];
    int raam;

    bool disco;
    clock_t discoTimer;

public:
    Muur(int, Appartement*);
    ~Muur();

    void operator()();
    nlohmann::json getStatus();
    bool updateStatus();
    void ToggleLed(int);
    bool isDisco();
    void RGBdimmen();
    void RGBaan();
    void RGBuit();
    void setDisco(bool);
    void LCDdimmen();
    void LCDdoorlaten();
    int potFilter(int);

    std::string arduinoStatus();
};
#endif
