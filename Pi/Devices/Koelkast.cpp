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
    int socketId = socket.getId();

    while (1) {
        socket.sendBuffer("getStatus\r");//Stuur getStatus naar device

        memset(buffer, 0, sizeof(buffer));
        if (!socket.receiveBuffer(buffer)) {
            std::cout << "Koelkast disconnected from socket: " << socketId << std::endl;
            close(socketId);
            return;
        }

        /* try and catch exceptions */
        try {
            auto j_koelkast = json::parse(buffer);

            /* store values extracted from json */
            koelkastDeur = j_koelkast.at("deur");
            NTC1 = j_koelkast.at("NTC1");
            NTC2 = j_koelkast.at("NTC2");
        }
        catch (json::exception& e) {
            std::cout << "Exception error at koelkast: " << e.what() << std::endl;
        }

        if (koelkastDeur && koelAlarm) {
            setKoelAlarm(false);
        }

        if (!koelkastDeur && !compareTime(timer, 5.0)) {
            setPeltier(false);
        } else if (!koelkastDeur && compareTime(timer, 5.0)) {
            setKoelAlarm(true);
            setFan(false);
            setPeltier(false);
        } else {
            timer = std::clock();
            setPeltier(true);
            setFan(true);
        }
        tempOut = calculateCelsius(NTC1);
        tempIn = calculateCelsius(NTC2);
    }
    /* close socket connection before the thread exits the function */
    close(socketId);
    std::cout << "Connection closed on socket " << socketId << std::endl;
}

json Koelkast::getStatus()//GetStatus voor de GUI
{
    /* store local variables in json object and return it */
    json koelk;
    koelk["Koelkast"] = {{"Deur", koelkastDeur}, {"Koelelement", koelelement}, {"m1", tempOut}, {"m2", tempIn}, {"Fan", fan}, {"Alarm", koelAlarm}};
    return koelk;
}

void Koelkast::setKoelAlarm(bool x)//Koelalarm uit
{
    /* mutex lock */
    const std::lock_guard<std::mutex>lock (koel_mutex);
    koelAlarm = x;
}

void Koelkast::setFan(bool x) // Fan aan/uit
{
    /* mutex lock */
    const std::lock_guard<std::mutex> lock (fan_mutex);
    if (x) {
        socket.sendBuffer("fanAan\r");
    } else {
        socket.sendBuffer("fanUit\r");
    }
    fan = x;
}

void Koelkast::setPeltier(bool x)//Peltier aan
{
    /* mutex lock */
    const std::lock_guard<std::mutex> lock (peltier_mutex);
    if (x) {
        socket.sendBuffer("peltierAan\r");
    } else {
        socket.sendBuffer("peltierUit\r");
    }
    koelelement = x;
}

float Koelkast::calculateCelsius(float i)//Berekent de temperatuur in Celcius
{
    //Formule gevonden op: https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html
    float C = 1.00 / (1.00 / 298.15 + 1.00 / 3380.00*(log (1023.00 / i - 1.00))) - 273.15;
    return C;
}
