/*
 * Stephanie Petrone
 * May 28, 2021
 *
 * Client program for TCP application
 * */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 128 //max size of message

int main(int argc, char *argv[])
{
	
	int sockid; //socket to connect to server
    struct sockaddr_in servAddr; //server address
	unsigned short portNum; //used for getting command line port
	char inputBuf[MAXLEN]; //buffer to sending strings
	char recBuf[MAXLEN]; //buffer for receiving echo
	int msgLength = 0; //length of message to send
	int count = 0; //number of bytes with send() and recv()

	//get command line arguments
	if(argc != 2)
	{
		printf("Use: ./program PORT\n");
	}

 	//get port from command line, turn into us
	portNum = atoi(argv[1]);
	memset(&servAddr, '\0', sizeof(servAddr));//initialize struct
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portNum);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//create socket
	if((sockid = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Error with socket(): \n");
		return -1;
	}

	//connect to server
	if(connect(sockid, (struct sockaddr *)&servAddr, sizeof(servAddr)) != 0)
	{
		perror("Error with connect(): \n");
		return -1;
	}


	//send message
	printf("\nType a message to send to echo server, or -1 to quit...\n");
	memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
	fgets(inputBuf, MAXLEN-1, stdin);
	msgLength = strlen(inputBuf);
	
	//set sentinal value
	if((atoi(inputBuf)) == -1)
		msgLength = -1;
	

	//enter loop to send and receive
	while(msgLength > 0)
	{
		if((count = send(sockid, inputBuf, msgLength, 0)) != msgLength)
		{
			perror("Error with send(): \n");
			return -1;
		}
		else
			printf("Sent message of length %d to server\n", count);

		//receive echo
		memset(recBuf, '\0', sizeof(recBuf)); //reset buffer
		count = recv(sockid, recBuf, sizeof(recBuf), 0);

		//print echo from server
		printf("\n\nECHO from server: \n");
		printf("length in bytes: %d\n", count);
		printf("%s\n", recBuf);

		//get new message or quit
		printf("\nType a message to send to echo server, or -1 to quit...\n");
		memset(inputBuf, '\0', sizeof(inputBuf)); //set buffer to 0
		fgets(inputBuf, MAXLEN-1, stdin);
		msgLength = strlen(inputBuf);
		
		//set sentinal value
		if((atoi(inputBuf)) == -1)
			msgLength = -1;

	}

	//close 
 	close(sockid); 

};
