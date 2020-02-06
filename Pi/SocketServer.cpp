#include "SocketServer.hpp"

using namespace std;

#define PORT 8883

SocketServer::SocketServer(const Appartement& appartement)
{
    a = appartement;
    startServer();
}

SocketServer::~SocketServer()
{}

void SocketServer::startServer()
{
    /* server_addr is struct in which the address is stored, defined in netinet.h and sys/socket */
    server_addr = {.sin_family = AF_INET, .sin_port = htons(PORT)};
    socksize = sizeof(server_addr);

    /* create socket */
    /* AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket" << endl;
    }

    /* bind connection to socket */
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        cout << "Error binding socket" << endl;
    }

    /* listen */
    /* make socket passive, meaning it will start listening and accept connections through accept() */
    if (listen(sockfd, 9) < 0) {
        cout << "Error listening" << endl;
    }

    /* start accepting connections in a new thread */
    thread acceptCon(&SocketServer::acceptConnection, this);
    /* detach thread so that it can run indepentently */
    acceptCon.detach();
}

void SocketServer::acceptConnection()
{
    while(1) {
        if ((csock = accept(sockfd, (struct sockaddr*) &server_addr, &socksize)) != -1) {
	    /* resolve ip of peer for visual confirmation of connection */
            getpeername(csock, (struct sockaddr*) &server_addr, &socksize);
            char clientip[20];
            strcpy(clientip, inet_ntoa(server_addr.sin_addr));

            cout << "Accepting connection from " << clientip << " on socket " << csock << endl;

            /* perform handshake in a separate thread to avoid collisions */
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

    if (a.createDevice(sock, response[0])) {
        strcpy(response, "OK\r");
        send(sock, response, strlen(response), 0);
    } else {
        cout << "Wrong ID on socket " << sock << endl; close(sock);
    }
}
