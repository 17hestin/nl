#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define PORT 8080
#define BUFFER_SIZE 4
#define TIMEOUT_SECONDS 3
int send_and_receive_ack(char message[]) {
int sockfd;
struct sockaddr_in server_addr;
socklen_t server_len;
char ack[BUFFER_SIZE];
ssize_t sent_bytes, received_bytes;
fd_set readFds;
FD_ZERO(&readFds);
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("socket creation failed");
exit(EXIT_FAILURE);
}
int flags = fcntl(sockfd, F_GETFL, 0);
if (flags == -1) {
printf("Error getting socket flags\n");
close(sockfd);
return -1;
}
flags = flags | O_NONBLOCK;
if (fcntl(sockfd, F_SETFL, flags) == -1) {
printf("Error setting socket to non-blocking mode.\n");
close(sockfd);
return -1;
}
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(PORT);
if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
perror("invalid address or address not supported");
exit(EXIT_FAILURE);
}
sent_bytes = sendto(sockfd, message, strlen(message), 0, (const struct
sockaddr *)&server_addr, sizeof(server_addr));
if (sent_bytes == -1) {
perror("sendto failed");
exit(EXIT_FAILURE);
}
FD_SET(sockfd, &readFds);
struct timeval timeout;
timeout.tv_sec = TIMEOUT_SECONDS;
timeout.tv_usec = 0;
int selectResult = select(sockfd + 1, &readFds, NULL,NULL,
&timeout);
if (selectResult == -1) {
printf("Error in select.\n");
close(sockfd);
return -1;
}
else if (selectResult == 0) {
close(sockfd);
return -1;
}
else
{
server_len = sizeof(server_addr);
memset(ack, 0,BUFFER_SIZE);
received_bytes = recvfrom(sockfd, ack, BUFFER_SIZE, 0,
(struct sockaddr *)&server_addr, &server_len);
if (received_bytes == -1) {
perror("recvfrom failed");
exit(EXIT_FAILURE);
}
int ack_in_number;
sscanf(ack, "%d", &ack_in_number);
close(sockfd);
return ack_in_number;
}
}
void main()
{
char message[100];
printf("Enter the Message : ");
scanf("%[^\n]",message);
int cs_flag=0;
int f_number = 0;
int i = 0;
int len = strlen(message);
while(i!=len)
{
char frame[5];
frame[0] = message[i];
sprintf(frame + 1, "%d", f_number);
printf("sending the frame %d\n",f_number);
int ack = send_and_receive_ack(frame);
if(ack == f_number)
{
printf("Acknowledgement for frame %d is received\n",f_number);
f_number++;
i++;
}
else
{
printf("Acknowledgement for frame %d is not received\n re",f_number);
}
}
printf("Data Tranfer is completed\n");
}
