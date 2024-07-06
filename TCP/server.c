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
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[BUFFER_SIZE] = {0};
char *hello = "Message recieved";
printf("Server is listening.......\n");
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
{
perror("socket failed");
exit(EXIT_FAILURE);
}
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);

if (bind(server_fd, (struct sockaddr *)&address,
sizeof(address)) < 0) {
perror("bind failed");
exit(EXIT_FAILURE);
}
if (listen(server_fd, 3) < 0) {
perror("listen failed");
exit(EXIT_FAILURE);
}
if ((new_socket = accept(server_fd, (struct sockaddr
*)&address, (socklen_t *)&addrlen)) < 0) {
perror("accept failed");
exit(EXIT_FAILURE);
}
valread = read(new_socket, buffer, BUFFER_SIZE);
printf("Received message from client: %s\n", buffer);
send(new_socket, hello, strlen(hello), 0);
printf("Response send to client\n");
close(new_socket);
return 0;
}
