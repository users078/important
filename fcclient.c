#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"
int main() {
 int socketDescriptor;
 struct sockaddr_in serverAddress;
 char sendBuffer[1000], recvBuffer[1000];
 pid_t cpid;
 bzero(&serverAddress, sizeof(serverAddress));
 serverAddress.sin_family = AF_INET;
 serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
 serverAddress.sin_port = htons(5500);
 /* Creating a socket, assigning IP address and port number for that socket */
 socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
 /* Check for socket creation errors */
 if (socketDescriptor == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 /* Connect establishes connection with the server using server IP address */
 if (connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
 perror("Connection failed");
 close(socketDescriptor); // Close the socket in case of connection failure
 exit(EXIT_FAILURE);
 }
 /* Fork is used to create a new process */
 cpid = fork();
 if (cpid == 0) {
 while (1) {
 bzero(&sendBuffer, sizeof(sendBuffer));
 printf("\nType a message here ... ");
 /* This function is used to read from server */
 fgets(sendBuffer, sizeof(sendBuffer), stdin);
 /* Send the message to server */
 send(socketDescriptor, sendBuffer, strlen(sendBuffer) + 1, 0);
 printf("\nMessage sent!\n");
 }
 } else {
 while (1) {
 bzero(&recvBuffer, sizeof(recvBuffer));
 /* Receive the message from server */
 recv(socketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
 printf("\nSERVER: %s\n", recvBuffer);
 }
 // Close the socket when the parent process terminates
 close(socketDescriptor);
 }
 return 0;
}
