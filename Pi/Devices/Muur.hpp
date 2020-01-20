#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"
#include "../Componenten/RGB.hpp"

class Muur: public Device
{
    static const int LAMPEN = 3;

    int ldr;
    int pot;
    RGBLed lampen[LAMPEN];
    int raam;

    bool disco;
    clock_t discoTimer;

public:
    Muur(int, Appartement*);
    ~Muur();
    bool ldrOverride = false;

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
    void setBrightness(bool);

    std::string arduinoStatus();
};
#endif
