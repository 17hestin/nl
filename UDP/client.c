#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 5000
#define BUFFER_SIZE 1024
int main() {
int sockfd;
struct sockaddr_in server_addr;
socklen_t server_len;
char buffer[BUFFER_SIZE];
ssize_t sent_bytes, received_bytes;
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("socket creation failed");
exit(EXIT_FAILURE);
}
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr))
<= 0) {
perror("invalid address or address not supported");
exit(EXIT_FAILURE);
}
char message[100];
printf("Enter the Message to server : ");
scanf("%s",message);
sent_bytes = sendto(sockfd, message, strlen(message), 0,
(const struct sockaddr *)&server_addr, sizeof(server_addr));
if (sent_bytes == -1) {
perror("sendto failed");
exit(EXIT_FAILURE);
}
server_len = sizeof(server_addr);
memset(buffer, 0, BUFFER_SIZE);
received_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
(struct sockaddr *)&server_addr, &server_len);
if (received_bytes == -1) {
perror("recvfrom failed");
exit(EXIT_FAILURE);
}
printf("Response from server: %s\n", buffer);
close(sockfd);
return 0;
}
