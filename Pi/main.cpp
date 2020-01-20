#include "Appartement.hpp"
#include "SocketServer.hpp"

int main()
{
    Appartement a;      // create new appartement object
    SocketServer s(a);  // create new socketserver object and pass appartement

    while(1)
    {}
}
