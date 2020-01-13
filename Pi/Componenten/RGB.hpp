#ifndef RGB_HPP
#define RGB_HPP

#include <string>
#include "../nlohmann/json.hpp"

class RGB {
public:
    int red;
    int green;
    int blue;
    void setRed(int);
    void setGreen(int);
    void setBlue(int);
    RGB(int r,int g,int b):red(r),green(g),blue(b){};
};

class RGBLed {

public:
    void setOld() {
        old->red   = rgb->red;
        old->green = rgb->green;
        old->blue  = rgb->blue;
    }
    void getOld() {
        rgb->red   = old->red;
        rgb->green = old->green;
        rgb->blue  = old->blue;
    }
    RGB* rgb;
    RGB* old;
    RGBLed():rgb(new RGB(0,0,0)),old(new RGB(0,0,0)){}

    void setKleur(int r, int g , int b) {
        rgb->red   = r;
        rgb->green = g;
        rgb->blue  = b;
    }

    void uit() {
        if (isOn() && !isOff()) {
            setOld();
            setKleur(0,0,0);
        }
    }

    void aan() {
        if (isOff() && !isOn()) {
            getOld();
        }
    }

    nlohmann::json getKleur() {
        return {{"R", rgb->red},{"G", rgb->green},{"B",rgb->blue}};
    }

    bool isOn() {
        if (rgb->red == 0 && rgb->blue == 0 && rgb->green == 0) {
            return false;
        }

        return true;
    }

    bool isOff() {
        if (rgb->red != 0 || rgb->blue != 0 || rgb->green != 0) {
            return false;
        }

        return true;
    }
};
#endif
