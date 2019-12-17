#include "Server.h"
#include "Thread.h"

using namespace std;

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
    Bed* b = new Bed(0);
    Gui g(0);

    while(1) {
        /* accept connections en geef ze een thread */
        if ((csock = accept(sockfd, (struct sockaddr*) &server_addr, &socksize)) != -1) {

            getpeername(csock, (struct sockaddr*) &server_addr, &socksize);
            char clientip[20];
            strcpy(clientip, inet_ntoa(server_addr.sin_addr));

            cout << "Accepting connection from: " << clientip << ", on socket number: " << csock << endl;

            char response[100];

            strcpy(response, "ID?");
            sendMsg(csock, response);

            memset(response, 0, sizeof(response));

            recvMsg(csock, response);

           // Bed* b = new Bed(csock);

            if (response[0] == 'x') {
                g.setSock(csock);
                g.setBed(b);
                thread socketThread(&Gui::operator(), g);
                socketThread.detach();

                strcpy(response, "OK");
                sendMsg(csock, response);

                cout << "GUI connected to socket number " << csock << endl;
            }
            else if (response[0] == 'y') {
                b->setSock(csock);
                thread socketThread(&Bed::operator(), b);
                socketThread.detach();

                strcpy(response, "OK");
                sendMsg(csock, response);

                cout << "Bed connected to socket number " << csock << endl;
            }
            else {
                strcpy(response, "Unknown ID");
                sendMsg(csock, response);
                close(csock);

                cout << "Socket number " << csock << " closed. (Unknown ID)" << endl;
            }
        }

        /* als -1 */
        else {
            cout << "Error accepting connection" << endl;
        }
    }
}


void Server::sendMsg(int sock, char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        cout << "Error sending" << endl;
    }
}

bool Server::recvMsg(int sock, char* data)
{
    return (recv(sock, data, 255, 0) > 0);
}
