#include "Server.h"
#define PORT 8883

Server::Server()
{
    setupServer();
}

void Server::setupServer()
{
    server_addr = {.sin_family = AF_INET, .sin_port = htons(PORT)};
    socksize = sizeof(server_addr);

    /* create de socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket" << endl;
    }

    /* bind de socket */
    if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        cout << "Error binding socket" << endl;
    }

    /* luister naar de sockets, 5 = max queue */
    if (listen(sockfd, 5) < 0) {
        cout << "Error listening" << endl;
    }

    /* het programma wacht bij accept(), daarom krijgt deze functie een aparte thread */
    thread threadManager(&Server::acceptConnection, this);
    threadManager.detach();
}

void Server::acceptConnection()
{
    while(1) {
        /* accept connections en geef ze een thread */
        if ((csock = accept(sockfd, (struct sockaddr*) &server_addr, &socksize)) != -1) {
            cout << "Accepting connection" << endl;

            thread socketThread(&Server::threadHandler, this, csock);
            socketThread.detach();
        }

        /* als -1 */
        else {
            cout << "Error accepting connection" << endl;
        }
    }
}

void Server::threadHandler(int sock)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    /* zolang de client connected is */
    while (recvMessage(buffer, sock)) {
        cout << buffer << endl;
        cout << sock << endl;

        strcat(buffer, "echo");

        sendMessage(buffer, sock);

        memset(buffer, 0, sizeof(buffer));
    }
    /* close de socket als een client disconnect zodat deze opnieuw gebruikt kan worden */
    close(sock);
}

void Server::sendMessage(char* buffer, int sock)
{
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        cout << "Error sending" << endl;
    }
}

bool Server::recvMessage(char* buffer, int sock)
{
    return (recv(sock, buffer, 255, 0) > 0);
}
