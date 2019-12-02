#include <cstring> // memset, strcpy
#include <netdb.h> // socket functies
#include <sys/socket.h> // socket functies
#include <unistd.h> // read, send
#include <iostream> // cout

#include "communicator.h"

#define PORT 8888

using namespace std;

char* Communicator::retrieveValue(const char *target, char *data) {
    int sockfd, n;

    struct addrinfo *result;
    struct addrinfo hints;

    struct sockaddr_in *server_addr;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;

    if ((getaddrinfo(target, NULL, &hints, &result)) < -1) {
        cout << "getaddrinfo error" << endl;
        exit(1);
    }

    server_addr = (struct sockaddr_in *) result->ai_addr;
    server_addr->sin_port = htons(PORT);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error opening socket" << endl;
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) server_addr, sizeof(*server_addr)) < 0) { 
        cout << "Error connecting" << endl;;
        exit(1);
    }

    if ((n = send(sockfd, data, strlen(data), 0)) < 0) { 
        cout << "Error writing to socket" << endl;
        exit(1);
    }

    memset(data, 0, sizeof(data));

    cout << "1" << endl;
    if ((n = read(sockfd, data, 63)) < 0) {
        cout << "Error reading from socket" << endl;
    }
    cout << "2" << endl;

    close(sockfd);

    return data;
}
