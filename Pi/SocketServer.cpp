#include "SocketServer.hpp"

using namespace std;

#define PORT 8883

SocketServer::SocketServer(Appartement* a)
{
    appartement = a;
    startServer();
}

void SocketServer::startServer()
{
    server_addr = {.sin_family = AF_INET, .sin_port = htons(PORT)};
    socksize = sizeof(server_addr);

    /* create socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket" << endl;
    }

    /* bind socket */
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        cout << "Error binding socket" << endl;
    }

    /* listen to socket */
    if (listen(sockfd, 5) < 0) {
        cout << "Error listening" << endl;
    }

    /* start accepting connections in a separate thread */
    thread acceptConnections(&SocketServer::acceptConnection, this);
    acceptConnections.detach();
}

void SocketServer::acceptConnection()
{
    while(1)
    {
        if ((csock = accept(sockfd, (struct sockaddr*) &server_addr, &socksize)) != -1) {
                /* get peer IP */
                getpeername(csock, (struct sockaddr*) &server_addr, &socksize);
                char clientip[20];
                strcpy(clientip, inet_ntoa(server_addr.sin_addr));

                cout << "Accepting connection from: " << clientip << ", on socket: " << csock << endl;
                thread handshake(&SocketServer::handshake, this, csock);
        }
        else {
            cout << "Error accepting connection" << endl;
        }
    }
}

void SocketServer::handshake(int sock)
{
    char response[100];
    strcpy(response, "ID?");

    /* send */
    if (send(sock, response, strlen(response), 0) < 0) {
        cout << "Error sending" << endl;
    }

    memset(response, 0, sizeof(response));

    /* receive */
    if (recv(sock, response, 99, 0) < 0) {
        cout << "Error receiving" << endl;
    }

    switch(response[0]) {
        case 'x': appartement->createDevice("GUI"); break;
        case 'y': appartement->createDevice("Bed"); break;
        case 'z': appartement->createDevice("Stoel"); break;
        default: close(sock); cout << "Socket number " << csock << " closed. (Unknown ID)" << endl; break;
    }
}
