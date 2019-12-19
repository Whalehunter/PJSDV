#include "SocketServer.hpp"
#include "Appartement.hpp"
#include "Device.hpp"

using namespace std;

int main()
{
    Appartement* appartement = new Appartement;
    SocketServer server(appartement);

    while(1)
    {

    }
}
