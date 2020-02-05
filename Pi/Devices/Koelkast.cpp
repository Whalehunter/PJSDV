#include "Koelkast.hpp"

using json = nlohmann::json;

Koelkast::Koelkast(int n, Appartement* ap): Device(n, ap)//Koelkast aanmaken
{
    std::cout << "Koelkast aangemaakt" << std::endl;
}

Koelkast::~Koelkast()//Koelkast vernietigen
{}

void Koelkast::operator()()//Operatie functie van koelkast
{
    char buffer[256] = {0};

    while (1) {
        sendMsg("getStatus\r");//Stuur getStatus naar device

        memset(buffer, 0, sizeof(buffer));
        if (!recvMsg(buffer)) {
            std::cout << "Koelkast disconnected from socket: " << sock << std::endl;
            close(sock);
            return;
        }

        try {
            auto j_koelkast = json::parse(buffer);

            koelkastDeur = j_koelkast.at("deur");
            NTC1 = j_koelkast.at("NTC1");
            NTC2 = j_koelkast.at("NTC2");
        }
        catch (json::exception& e) {
            std::cout << "Exception error at koelkast: " << e.what() << std::endl;
        }

        if (koelkastDeur && koelAlarm) {
            disableKoelAlarm();
        }

        if ((koelkastDeur == 0) && (((std::clock() - timer) / (double) CLOCKS_PER_SEC) <= 5.0)) {
            setPeltier(false);
        }
        else if ((koelkastDeur == 0) && (((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= 5.0)) {
            koelAlarm = 1;
            setFan(false);
            setPeltier(false);
        }
        else {
            timer = std::clock();
            setPeltier(true);
            setFan(true);
        }
        tempOut = calculateCelsius(NTC1);
        tempIn = calculateCelsius(NTC2);
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
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

void Koelkast::setFan(bool x) // Fan aan/uit
{
    const std::lock_guard<std::mutex> lock (fan_mutex);
    if (x) {
        sendMsg("fanAan\r");
    } else {
	sendMsg("fanUit\r");
    }
    fan = x;
}

void Koelkast::setPeltier(bool x)//Peltier aan
{
    const std::lock_guard<std::mutex> lock (peltier_mutex);
    if (x) {
	sendMsg("peltierAan\r");
    } else {
	sendMsg("peltierUit\r");
    }
    koelelement = x;
}

float Koelkast::calculateCelsius(float i)//Berekent de temperatuur in Celcius
{
    //Formule gevonden op: https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html
    float C = 1.00 / (1.00 / 298.15 + 1.00 / 3380.00*(log (1023.00 / i - 1.00))) - 273.15;
    return C;
}
