#include "Muur.hpp"
#include "Deur.hpp"

using json = nlohmann::json;
using namespace std;

Muur::Muur(int n, Appartement* ap): Device(n, ap), /*state(LCDDOORLATEN),*/ ldr(0), pot(0)
{
    std::cout << "Muur aangemaakt" << std::endl;
}

Muur::~Muur()
{
}

void Muur::operator ()()
{
    while(1){
        /*Get and store JSON values*/
        if (!updateStatus()) break;

        /*checks*/
        if (ldr >= 500){
            LCDdoorlaten();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampUit();
            }
        }
        else if (ldr < 500){
            LCDdimmen();
            if (a->devices.count('d')) {
                dynamic_cast<Deur *>(a->devices.find('d')->second)->binnenLampAan();
            }

        }
        /*Sends value of pot to WEMOS*/
        RGBdimmen();
    }
}

nlohmann::json Muur::getStatus()
{
    json muurData = {/*{"Muur", state ? "LcdDimmen" : "LcdDoorlaten"},*/ {"LDR", ldr}, {"POT", pot}};

    return muurData;
}

bool Muur::updateStatus()
{
    char buffer[256];

    sendMsg("getStatus\r");
    memset(buffer, 0, sizeof(buffer));
    if(recv(sock, buffer, 255, 0) < 1){
        std::cout << "Muur disconnected from socket: " << sock << std::endl;
        close(sock);
        return false;
    }

    try {
        auto j_muur = json::parse(buffer);

        ldr = j_muur.at("ldr");
        pot = j_muur.at("pot");
    }
    catch(json::exception& e){
        std::cout << "Parsing error at Muur on socket " << sock << std::endl;
    }
    return true;
}

void Muur::LCDdimmen()
{
    sendMsg("dimmen\r");
}

void Muur::LCDdoorlaten()
{
    sendMsg("doorlaten\r");
}


void Muur::RGBdimmen()
{
    stringstream p;
    p << pot;
    p << "\r";
    string str = p.str();
    char* tempc = (char*) str.c_str();

    sendMsg(tempc);

    if (pot == 0){
        RGBuit();
    }
    else {
        RGBaan();
    }
}

void Muur::RGBaan()
{
    sendMsg("RGBaan\r");
}

void Muur::RGBuit()
{
    sendMsg("RGBuit\r");
}

void Muur::RGBdisco()
{
    sendMsg("Disco\r");
}
