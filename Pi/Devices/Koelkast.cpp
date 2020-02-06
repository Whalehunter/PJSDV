#include "Koelkast.hpp"

using json = nlohmann::json;

Koelkast::Koelkast(int n, Appartement* ap): Device(n, ap), openTimer(0)//Koelkast aanmaken
{
    std::cout << "Koelkast aangemaakt" << std::endl;
}

Koelkast::~Koelkast()//Koelkast vernietigen
{}

void Koelkast::operator()()//Operatie functie van koelkast
{
    char buffer[256] = {0};
    int socketId = socket.getId();

    while (1) {
        socket.sendBuffer("getStatus\r");//Stuur getStatus naar device

        memset(buffer, 0, sizeof(buffer));
        if (!socket.receiveBuffer(buffer)) {
            std::cout << "Koelkast disconnected from socket: " << socketId << std::endl;
            close(socketId);
            return;
        }

        try {
            auto j_koelkast = json::parse(buffer);

            koelkastDeur = j_koelkast.at("deur");
            NTC1 = j_koelkast.at("NTC1");
            NTC2 = j_koelkast.at("NTC2");
        }
        catch (json::parse_error) {
            std::cout << "parse error" << std::endl;
        }

        if (koelkastDeur && koelAlarm) {
            disableKoelAlarm();
        }

        if ((koelkastDeur == 0) && (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) <= 5.0)) {
            peltierUit();
        }
        else if ((koelkastDeur == 0) && (((std::clock() - openTimer) / (double) CLOCKS_PER_SEC) >= 5.0)) {
            koelAlarm = 1;
            fanUit();
            peltierUit();
        }
        else {
            openTimer = std::clock();
            peltierAan();
            fanAan();
        }
        tempOut = calculateCelsius(NTC1);
        tempIn = calculateCelsius(NTC2);
    }
    close(socketId);
    std::cout << "Connection closed on socket " << socketId << std::endl;
}

json Koelkast::getStatus()//GetStatus voor de GUI
{
    json koelk;
    koelk["Koelkast"] = {{"Deur", koelkastDeur}, {"Koelelement", koelelement}, {"m1", tempOut}, {"m2", tempIn}, {"Fan", fan}, {"Alarm", koelAlarm}};
    return koelk;
}

void Koelkast::disableKoelAlarm()//Koelalarm uit
{
    koelAlarm = 0;
}

void Koelkast::fanAan()//Fan aan
{
    socket.sendBuffer("fanAan\r");
}

void Koelkast::fanUit()//Fan uit
{
    socket.sendBuffer("fanUit\r");
}

void Koelkast::peltierAan()//Peltier aan
{
    socket.sendBuffer("peltierAan\r");
    koelelement = 1;
}

void Koelkast::peltierUit()//Peltier uit
{
    socket.sendBuffer("peltierUit\r");
    koelelement = 0;
}

float Koelkast::calculateCelsius(float i)//Berekent de temperatuur in Celcius
{
    //Formule gevonden op: https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html
    float C = 1.00 / (1.00 / 298.15 + 1.00 / 3380.00*(log (1023.00 / i - 1.00))) - 273.15;
    return C;
}
