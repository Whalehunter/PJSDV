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

};

class RGBLed {
    void setOld() {
        old->red   = rgb->red;
        old->green = rgb->green;
        old->blue  = rgb->blue;
    }
public:
    RGB* rgb;
    RGB* old;
    RGBLed():rgb(new RGB()),old(new RGB()){
        rgb->red = 0;
        rgb->green = 0;
        rgb->blue = 0;
    }

    void setKleur(int r, int g , int b) {
        setOld();

        rgb->red = r;
        rgb->green = g;
        rgb->blue = b;
    }

    void toggle() {
        int r = old->red;
        int g = old->green;
        int b = old->blue;

        old->red = rgb->red;
        old->green = rgb->green;
        old->blue = rgb->blue;

        rgb->red = r;
        rgb->green = g;
        rgb->blue = b;
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
