#ifndef RGB_HPP
#define RGB_HPP

#include <string>
#include <ctime>
#include "../nlohmann/json.hpp"

class RGB {
public:
    int red;
    int green;
    int blue;
    int brightness = 255;
    int polPot;
    bool lock;
    RGB(int r,int g,int b):red(r),green(g),blue(b){};
};

class RGBLed {
public:
    std::string currentDiscoColor = "rood";
    RGB* rgb;
    RGB* old;
    RGBLed():rgb(new RGB(0,0,0)),old(new RGB(0,0,0)){}

    int getDiscoColor(std::string kleur) {
        if (currentDiscoColor == kleur)
            return rgb->brightness;
        return 0;
    }
    void updateDiscoColor() {
        lock = true;
        if (!lock) {
            if (currentDiscoColor == "rood") {
                currentDiscoColor = "groen";
            } else if (currentDiscoColor == "groen") {
                currentDiscoColor = "blauw";
            } else {
                currentDiscoColor = "rood";
            }
            lock = false;
        }
    }
    void setBrightness(int pot) {
        rgb->brightness = pot;
    }
    int getBrightness() {
        return rgb->brightness;
    }
    void setKleur(int r, int g , int b) {
        rgb->red   = r;
        rgb->green = g;
        rgb->blue  = b;
    }
    void uit() {
        if (isOn() && !isOff()) {
            setBrightness(0);
        }
    }
    void aan() {
        if (isOff() && !isOn()) {
            setBrightness(255);
        }
    }
    nlohmann::json getKleur(bool disco = false) {
        if (!disco) return {{"R", rgb->brightness},{"G", rgb->brightness},{"B",rgb->brightness}};
        return {{"R", getDiscoColor("rood")}, {"G", getDiscoColor("groen")}, {"B", getDiscoColor("blauw")}};
    }
    bool isOn() {
        if (rgb->brightness == 0) return false;
        return true;
    }
    bool isOff() {
        if (rgb->brightness != 0) return false;
        return true;
    }
};
#endif
