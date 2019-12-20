#include "SocketServer.hpp"

using namespace std;

#define PORT 8883

SocketServer::SocketServer(const Appartement& appartement)
{
    a = appartement;
    startServer();
}

SocketServer::~SocketServer()
{
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

    /* listen */
    if (listen(sockfd, 5) < 0) {
        cout << "Error listening" << endl;
    }

    thread acceptCon(&SocketServer::acceptConnection, this);
    acceptCon.detach();
}

void SocketServer::acceptConnection()
{
    while(1) {
        if ((csock = accept(sockfd, (struct sockaddr*) &server_addr, &socksize)) != -1) {
            getpeername(csock, (struct sockaddr*) &server_addr, &socksize);
            char clientip[20];
            strcpy(clientip, inet_ntoa(server_addr.sin_addr));

            cout << "Accepting connection from " << clientip << " on socket " << csock << endl;

            thread handshakeThread(&SocketServer::handshake, this, csock);
            handshakeThread.detach();
        }
        else {
            cout << "Error accepting connection" << endl;
        }
    }
}

void SocketServer::handshake(int sock)
{
    char response[100];
    strcpy(response, "ID?\r");

    if (send(sock, response, strlen(response), 0) < 0) {
        cout << "Error sending" << endl;
    }

    memset(response, 0, sizeof(response));

    if (recv(sock, response, 99, 0) < 0) {
        cout << "Error listening" << endl;
    }

    switch(response[0]) {
        case 'x':
        case 'y':
        case 'z': a.createDevice(sock, response[0]);
                  strcpy(response, "OK\r");
                  send(sock, response, strlen(response), 0); break;
        default: cout << "Wrong ID on socket " << sock << endl; close(sock); break;
    }
}
