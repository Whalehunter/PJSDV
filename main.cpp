#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <netinet/in.h>

#define PORT 8889

using namespace std;

void threadHandler(int csock);

int main()
{
    int csock, sockfd; // client socket, server socket

    struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_port = htons(PORT)};
    unsigned int socksize = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket" << endl;
    }

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cout << "Error binding socket" << endl;
    }

    if (listen(sockfd, 5) < 0) {
        cout << "Error listening" << endl;
    }

    while (1) {
        if ((csock = accept(sockfd, (struct sockaddr *) &server_addr, &socksize)) != -1) {
            cout << "Accepting connection" << endl;

            std::thread th(&threadHandler, csock);
            th.detach();
        }
        else {
            cout << "Error accepting connection" << endl;
        }
    }
}




void threadHandler(int csock)
{
    char buffer[256];
    int n; // error handling
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        if ((n = recv(csock, buffer, 255, 0)) < 0) {
            cout << "Error receiving" << endl;
        }

        cout << buffer << endl;
        cout << csock << endl;

        strcat(buffer, "echo");

        if ((n = send(csock, buffer, strlen(buffer), 0)) < 0) {
            cout << "Error sending" << endl;
        }
    }
}


