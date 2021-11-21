#include<stdio.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h> // starlen
#include<unistd.h> // write
#include<net/if_arp.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#define PORT 20
#define SIZE 1024



void write_file(int sockfd);


int main(int argc , char *argv[])
{
	//declaration
	int socket_desc, new_socket , new_sock;
	struct sockaddr_in server, new_addr; 
	socklen_t addr_size;
	char buffer[SIZE];
	char fname[50];
	FILE *fp;
	
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("[-]Could not create socket");
	}
	
	puts("[+]Socket created\n");
	
	server.sin_addr.s_addr = INADDR_ANY; //Please enter the ip address of your Server VM
	server.sin_family = AF_INET;
	server.sin_port = htons( PORT );


	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//puts("bind failed. Error");
		printf("Error binding socket to port %d.\n", PORT);
		return 1;
	}
	puts("[+]Binding successfull.");

	
	
	//Listen
	listen(socket_desc , 3);


	if (new_socket<0)
	{
		perror("[-]Accept failed");
		return 1;
	}

	addr_size = sizeof(new_addr);
	new_sock = accept(socket_desc, (struct sockaddr*)&new_addr, &addr_size);
	
	 
	write_file(new_sock);
	printf("[+]Send received successfully, please check directory.\n");
	printf("[+]Closing the connection.\n");



	close(socket_desc);
	return 0;
	
}

// Function receive file

void write_file(int sockfd)
{
  int i;
  FILE *fp;
  char *filename = "receivefromclient.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    i = recv(sockfd, buffer, SIZE, 0);
    if (i <= 0)
	{
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}
