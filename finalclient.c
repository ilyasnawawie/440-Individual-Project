#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<sys/stat.h>
#define BUFSIZE 128
#define SIZE 1024
#define PORT 20

int checkIfFileExists(const char *filename);
void write_file(int sockfd);
void send_file(FILE *fp, int sockfd);



int main(int argc , char *argv[])
{
    //declaration
    int socket_desc;//sockfd
    struct sockaddr_in server; //sockaddr_in server_addr
	FILE *fp;
	char command[50];
	int new_sock;
	char *filename = "sendtoserver.txt";


    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        puts("[-]Could not create socket.");
    }
    else 
    {
        puts("[+]Socket created.\n");
    }
	
    server.sin_addr.s_addr = inet_addr("192.168.56.4"); //Please enter the ip address of your Server VM
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("[-]Connect error.");
        return 1;
    }

    puts("[+]Connected \n");

	
	printf("Enter FTP command, Check // Create // Send: ");
	scanf("%s",command);


	if(strcmp(command, "check") == 0)

{
     if(checkIfFileExists("sendtoserver.txt"))
    {
        printf("[+]File exists. \n");
    }
    else
    {
        printf("[-]File does not exists.");
    }

}

else if(strcmp(command, "create") == 0)
	
{
	write_file(new_sock);
	printf("[+]Data written in the file successfully.\n");

}

	


else if (strcmp(command, "send") == 0)

{
fp = fopen(filename, "r");
	if (fp == NULL) 
	{
		perror("[-]Cannot read file.");
		exit(1);
	}
	//CALL SEND FILE FUNCTION
	send_file(fp, socket_desc);
	printf("[+]File sent successfully.\n");
}

	printf("[+]Closing the connection.\n");
	close(socket_desc);
    return 0;
	
}



// FUNCTIONS //


//Function to check file if send function works
int checkIfFileExists(const char* filename)
{
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else  
        return 0;
}

//Function to create file within client
void write_file(int sockfd)
{
  int k;
  FILE *fp;
  char *filename = "sendtoserver.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) 
  {
    k = recv(sockfd, buffer, SIZE, 0);
    if (k <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

//Function to send the file
void send_file(FILE *fp, int sockfd)
{
  int i;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("CANNOT SEND FILE.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

// END OF FUNCTIONS