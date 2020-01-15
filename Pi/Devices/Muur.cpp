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
        updateStatus();

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

void Muur::updateStatus()
{
    char buffer[256];

    sendMsg("getStatus\r");
    memset(buffer, 0, sizeof(buffer));
    if(recv(sock, buffer, 255, 0) < 1){
        std::cout << "Muur disconnected from socket: " << sock << std::endl;
        close(sock);
        return;
    }

    try {
        auto j_muur = json::parse(buffer);

        ldr = j_muur.at("ldr");
        pot = j_muur.at("pot");
    }
    catch(json::exception& e){
        std::cout << "Parsing error at Muur on socket " << sock << std::endl;
    }
}

void Muur::LCDdimmen()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "dimmen\r");
    sendMsg(buff);
}

void Muur::LCDdoorlaten()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "doorlaten\r");
    sendMsg(buff);
}


void Muur::RGBdimmen()
{
    stringstream p;
    p << pot;
    p << "\r";
    string str = p.str();
    char* tempc = (char*) str.c_str();

    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, tempc);
    sendMsg(buff);

    if (pot == 0){
        RGBuit();
    }
    else {
        RGBaan();
    }
}

void Muur::RGBaan()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "RGBaan\r");
    sendMsg(buff);
}

void Muur::RGBuit()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "RGBuit\r");
    sendMsg(buff);
}

void Muur::RGBdisco()
{
    char buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, "Disco\r");
    sendMsg(buff);
}
