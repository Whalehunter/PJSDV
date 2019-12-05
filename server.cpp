#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 60005
#define BUFLEN 1024

using std::string;

int main() {
    struct sockaddr_in serv_addr = {.sin_family = AF_INET, .sin_port = htons(PORT)};
    unsigned int socksize = sizeof(serv_addr);
    char buffer[BUFLEN];
    int sock, new_socket;
    char msg[BUFLEN] = "Hallo man";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Address binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(sock, (struct sockaddr*)&serv_addr, &socksize)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if (new_socket > 0) {
            read(new_socket, buffer, BUFLEN);
            printf("%s\n", buffer);
            send(new_socket, msg, BUFLEN, 0);
        }
    }

    close(new_socket);
    close(sock);
}
