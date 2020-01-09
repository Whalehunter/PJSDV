#include <string>

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
    bool power;
    int brightness;
public:
    RGB* rgb;
    RGBLed():rgb(new RGB()){}
    void setKleur(int, int, int);
};

void RGBLed::setKleur(int r, int g, int b) {
    rgb->red = r;
    rgb->green = g;
    rgb->blue = b;
}
