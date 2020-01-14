#include "Appartement.hpp"
#include "SocketServer.hpp"
//#include "nlohmann/json.hpp"

//using json = nlohmann::json;
//using namespace std;

int main()
{
    Appartement a;
    SocketServer s(a);

    while(1)
    {}
}
