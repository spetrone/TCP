/*
 * Stephanie Petrone
 * May 28, 2021
 *
 * Server program for TCP application 
   iterative server
 * */

#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 128  //max byte size of message
#define LISTEN_PORT 8989 //port TCP server will listen


int main()
{
	struct sockaddr_in clientAddr; //holds client address of sock
	int addrLen; //holds lenght of client adress
	int count = 0 ; //bytes received wirth recv()
	int connectSock; //new socket with connection
	int listenSock; //socket for listen()
	char buf[MAXLEN]; //buffer for receiving 
	struct sockaddr_in serverSockAddr;
	char IP_str[16]; //for holding string of IP addr (up tp 16 chars)
	
	//create socket
	listenSock = socket(PF_INET, SOCK_STREAM, 0);

    //set address
	memset(&serverSockAddr, '0', sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons(LISTEN_PORT);
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	//bind socket
	if( bind(listenSock, (struct sockaddr* )&serverSockAddr, sizeof(serverSockAddr)) == -1)
	{
        perror("Error with bind(): ");
		return -1;
	}
	else
	{
		//ip to string for printing
		memset(IP_str, '\0', sizeof(IP_str));
		strcpy(IP_str, inet_ntoa(serverSockAddr.sin_addr));
		
		printf("\nlistenSock binded to port %u\n", LISTEN_PORT);
		printf("\nIP address: %s\n\n", IP_str); 
	}


	//listen for connection
	if (listen(listenSock, 1) == -1)
	{
		perror("Error with listen(): ");
		return -1;
	}

	//print status
	printf("\nServer listening on port: %u...\n", LISTEN_PORT);

    
	//iteratively accept connection
	for(;;)
	{
		//accept new connection
		if(( connectSock = accept(listenSock,(struct sockaddr *) &clientAddr, &addrLen)) == -1)
		{
			perror("Error with accept(): ");
			return -1;
		}
		else printf("\nSuccesfully connected to client socket\n\n");

		//print client information

		
		//receive first message then loop for more receiving
		memset(buf, '\0', sizeof(buf)); //clear buffer
		if((count = recv(connectSock, buf, sizeof(buf), 0)) < 0)
		{
			perror("error with first recv(): ");
			return -1;
		}
				
		//loop for recv() more until nothing more is received
		while (count > 0)
		{
			//print in server the message received
			printf("\nMessage, size %d received into buf: \n %s\n\n",count, buf);

			//echo messsage
			if ((send(connectSock, buf, count, 0)) != count)
			{
				perror("Error with echo send(): ");
			}
			else
				printf("Sent message of size %d to client\n", count);

			//receive next message
			memset(buf, '\0', sizeof(buf)); //reset buffer
			count = recv(connectSock, buf, sizeof(buf), 0); 
			
			
		}

	//close client socket 
	close(connectSock);
	}
}
