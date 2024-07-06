#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
int sock = 0, valread;
struct sockaddr_in serv_addr;
char message[100];
char buffer[BUFFER_SIZE] = {0};

if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
perror("socket creation failed");
exit(EXIT_FAILURE);
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);

if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=
0) {
perror("invalid address or address not supported");
exit(EXIT_FAILURE);
}

if (connect(sock, (struct sockaddr *)&serv_addr,
sizeof(serv_addr)) < 0) {
perror("connection failed");
exit(EXIT_FAILURE);
}

printf("Enter the message : ");
scanf("%s",message);

send(sock, message, strlen(message), 0);
printf("Message sent to server\n");

valread = read(sock, buffer, BUFFER_SIZE);
printf("Received response from server: %s\n", buffer);

close(sock);

return 0;
}






