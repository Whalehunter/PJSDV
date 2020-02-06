#ifndef RGB_HPP
#define RGB_HPP

#include <string>
#include <ctime>
#include "../nlohmann/json.hpp"

/**
 * RGB Class
 *
 * Bezit de kleur waarden van een RGB Led in een device
 *
 * Wordt gebruikt in RGBLed om
 */
class RGB {
public:
    int red;
    int green;
    int blue;
    int brightness = 255;
    int pot;
    RGB(int r,int g,int b):red(r),green(g),blue(b){};
};

/**
 * RGBLed class
 *
 * API voor het wijzigen van de RGB kleuren en helderheid op een device
 */
class RGBLed {
public:
    std::string currentDiscoColor = "rood";
    RGB* rgb;
    RGBLed():rgb(new RGB(0,0,0)){}

    /**
     * Discokleur ophalen
     *
     * param kleur: Kleur om de huidige disco kleur mee te vergelijken om de juiste kleur op de RGB op te halen
     *
     * return integer: De helderheid van de kleur op de LED
     */
    int getDiscoColor(std::string kleur) {
        if (currentDiscoColor == kleur)
            return rgb->brightness;
        return 0;
    }

    /**
     * Discokleur wijzigen
     *
     * Wordt gebruikt in de disco stand om de juiste kleur weer te geven
     */
    void updateDiscoColor() {
        if (currentDiscoColor == "rood") {
            currentDiscoColor = "groen";
        } else if (currentDiscoColor == "groen") {
            currentDiscoColor = "blauw";
        } else {
            currentDiscoColor = "rood";
        }
    }

    /**
     * Set de helderheid van de RGB Led
     *
     * param pot: de helderheid
     */
    void setBrightness(int pot) {
        rgb->brightness = pot;
    }

    /**
     * Kleuren van LED wijzigen
     *
     * param r: Rode kleur helderheid
     * param g: Groene kleur helderheid
     * param b: Blauwe kleur helderheid
     */
    void setKleur(int r, int g , int b) {
        rgb->red   = r;
        rgb->green = g;
        rgb->blue  = b;
    }

    /**
     * RGB Led uitzetten
     */
    void uit() {
        if (isOn() && !isOff()) {
            setBrightness(0);
        }
    }

    /**
     * RGB Led aan zetten
     */
    void aan() {
        if (isOff() && !isOn()) {
            setBrightness(255);
        }
    }

    /**
     * Kleuren ophalen in format die verzonden kan worden aan de device en aan de GUI
     *
     * param disco: Indien er discostand is, alleen de kleur ophalen die wordt gewenst
     *
     * return json: JSON object met de rood, groen en blauw helderheid waarden
     */
    nlohmann::json getKleur(bool disco = false) {
        if (!disco) return {{"R", rgb->brightness},{"G", rgb->brightness},{"B",rgb->brightness}};
        return {{"R", getDiscoColor("rood")}, {"G", getDiscoColor("groen")}, {"B", getDiscoColor("blauw")}};
    }

    /**
     * Check of de RGB LED aan staat
     */
    bool isOn() {
        if (rgb->brightness == 0) return false;
        return true;
    }

    /**
     * Check of de RGB LED uit staat
     */
    bool isOff() {
        if (rgb->brightness != 0) return false;
        return true;
    }
};
#endif
