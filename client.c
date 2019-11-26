#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <netdb.h> 
#include <sys/socket.h> 
#include <unistd.h>

#define PORT 8888

/* In deze demo versie maak ik gebruik van het meegeven van argumenten tijdens de oproep van het programma. 
 * In de realiteit zullen we hier een functie van maken die op dezelfde manier met argument opgeroepen wordt.
 * Veel van de informatie komt van de usna.edu site als je er meer over wilt weten.
 */

int main(int argc, char *argv[])
{
  if (argc != 2) {
	 printf("Usage: ./filename <hostname>\n");
	 exit(1);
  }

  int sockfd, n;  
  char buffer[256]; /* hier wordt de (read en write) data in opgeslagen */

  struct addrinfo *result; /* hierin wordt het resultaat van de hostname lookup gestopt */
  struct addrinfo hints; /* relevante informatie m.b.t. de lookup */

  struct sockaddr_in *server_addr; /* informatie van de server wordt hierin opgeslagen */

  memset(&hints,0,sizeof(struct addrinfo)); /* vul hints met 0en */
  hints.ai_family = AF_INET; /* IPv4 adressen */

  if ((getaddrinfo(argv[1], NULL, &hints, &result)) < 0) { /* DNS lookup en slaat het resultaat op */
	 printf("getaddrinfo error\n");
	 exit(1);
  }

  server_addr = (struct sockaddr_in *) result->ai_addr; /* convert generic socket adres naar internet socket adres */
  server_addr->sin_port = htons(PORT); /* set de port */

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { /* maak socket aan (domain, type, protocol) en kijk vervolgens of het gelukt is */
	 printf("Error opening socket\n");
	 exit(1);
  }

  if (connect(sockfd, (struct sockaddr *) server_addr, sizeof(*server_addr)) < 0) { /* connect naar de server */
	 printf("Error connecting\n");
	 exit(1);
  }

  while(1) {
	 printf("Enter message (g = draaiknop waarde, h = led aan, j = led uit): ");
	 memset(buffer, 0, sizeof(buffer)); /* maak buffer leeg */
	 fgets(buffer, sizeof(buffer), stdin); /* stop input in buffer */

	 if ((n = send(sockfd, buffer, strlen(buffer), 0)) < 0) { /* stuur bericht */
		printf("Error writing to socket\n");
		exit(1);
	 }

	 memset(buffer, 0, sizeof(buffer)); /* maak buffer leeg */
	 if ((n = read(sockfd, buffer, 255)) < 0) { /* lees sockfd en sla het op in buffer */
		printf("Error reading from socket\n");
		exit(1);
	 }

	 printf("%s\n", buffer); /* print de inhoud van buffer */
  }

  close(sockfd); /* close de socket */

  return 0;
}

