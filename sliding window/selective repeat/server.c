#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
int sockfd;
struct sockaddr_in server_addr, client_addr;
socklen_t client_len;
char buffer[BUFFER_SIZE];
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("socket creation failed");
exit(EXIT_FAILURE);
}
memset(&server_addr, 0, sizeof(server_addr));
memset(&client_addr, 0, sizeof(client_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);
if (bind(sockfd, (const struct sockaddr *)&server_addr,
sizeof(server_addr)) < 0) {
perror("bind failed");
exit(EXIT_FAILURE);
}
printf("Server listening on port %d...\n", PORT);
while (1) {
int r_time = rand() % 5;
sleep(r_time);
memset(buffer, 0, BUFFER_SIZE);
client_len = sizeof(client_addr);
ssize_t msg_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
(struct sockaddr *)&client_addr, &client_len);
if (msg_len == -1) {
perror("recvfrom failed");
exit(EXIT_FAILURE);
}
const char *response = &buffer[1];
int f_number = 0;
sscanf(response, "%d", &f_number);
printf("Received frame from client: %c\n", buffer[0]);
ssize_t sent_bytes = sendto(sockfd, response, strlen(response),
0, (struct sockaddr *)&client_addr, client_len);
if (sent_bytes == -1) {
perror("sendto failed");
exit(EXIT_FAILURE);
}
printf("Acknowledement send to client\n");
}
close(sockfd);
return 0;
}

