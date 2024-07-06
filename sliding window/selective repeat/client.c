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

int ack_array[2][4];

int window_size =
4,a_get=0,r_send=0,w_pos=0,f_number=0,length=0;
int send_and_receive_ack(char message[]) 
{
	int sockfd;
	struct sockaddr_in server_addr;
	socklen_t server_len;
	char ack[BUFFER_SIZE];
	ssize_t sent_bytes, received_bytes;
	fd_set readFds;
	FD_ZERO(&readFds);
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags == -1) 
	{
		printf("Error getting socket flags\n");
		close(sockfd);
		return -1;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <=0) 
	{
		perror("invalid address or address not supported");
		exit(EXIT_FAILURE);
	}
	sent_bytes = sendto(sockfd, message, strlen(message), 0,(const struct sockaddr *)&server_addr, sizeof(server_addr));
	if (sent_bytes == -1) 
	{
		perror("sendto failed");
		exit(EXIT_FAILURE);
	}
FD_SET(sockfd, &readFds);
struct timeval timeout;
timeout.tv_sec = TIMEOUT_SECONDS;
timeout.tv_usec = 0;
int selectResult = select(sockfd + 1, &readFds,
NULL,NULL, &timeout);
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
void slide()
{
for(int i=0;i<window_size;i++)
{
ack_array[0][i] = ack_array[0][i+1];
ack_array[1][i] = ack_array[1][i+1];
}
}
void send_window(char message[])
{
char frame[5];
for(int i=0;i<window_size && i<r_send;i++)
{
frame[0] = message[f_number];
sprintf(frame + 1, "%d", f_number);
printf("sending the frame %d\n",f_number);
ack_array[0][i] = f_number;
ack_array[1][i] = send_and_receive_ack(frame);
f_number++;
}
}

void get_ack_send(char message[])
{
int i=0;
char frame[5];
while(a_get!=length)
{ 
if(ack_array[1][i]!=-1)
{
printf("Acknowledgement for frame %d is recived\n",ack_array[0][i]);
r_send--;
a_get++;
if(r_send<window_size)
{
i++;
w_pos++;
}
else
{
slide();
w_pos++;
frame[0] = message[f_number];
sprintf(frame + 1, "%d", f_number);
printf("sending the frame %d\n",f_number);
ack_array[0][window_size-1] = f_number;
ack_array[1][window_size-1] = send_and_receive_ack(frame);
f_number++;
}
}
else
{
printf("Acknowledgement for frame %d is not received resending the frame\n",ack_array[0][i]);
int m_index = ack_array[0][i];
frame[0] = message[m_index];
sprintf(frame + 1, "%d", m_index);
printf("Resending the frame %d\n",m_index);
ack_array[1][i] = send_and_receive_ack(frame);
}
}
printf("Data Transfer Completed\n");
}
void main()
{
printf("Window Size is 4 frames\n");
char message[100];
printf("Enter the Message : ");
scanf("%[^\n]",message);
int len = strlen(message);
r_send = len;
length = len;
send_window(message);
get_ack_send(message);
}

