#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>
#include<sys/time.h>

#define SER_PORT 5020
struct data
{
	int i;
	char c;
	float f;
};
float htonf(float host);
int hostname_to_ip(char * , char *);

int main(int argc , char *argv[])
{

//If a host name is not entered prompt the user to enter one 
//and close the program
if(argc < 2)
{
printf("Please enter a hostname to resolve:\nRestart the program and try again");
exit(1);
}

char *hostname = argv[1];	// holds hostname
char ip[100];	// hold ip address
hostname_to_ip(hostname , ip); //resolves ip address
printf("%s resolved to %s" , hostname , ip);
printf("\n");
struct data dataToTransmit;
int connectionSocket;
struct sockaddr_in clientAddr;

clientAddr.sin_port=htons(SER_PORT);// gets the port number
clientAddr.sin_family=AF_INET;
clientAddr.sin_addr.s_addr=inet_addr(ip);

//Creates the socket for connection to Jason
printf("Creating a socket...");
connectionSocket=socket(AF_INET,SOCK_STREAM,0); 
if(connectionSocket<0)
{
perror("\n Error SOCKET could not be created:");
exit(0);
}


printf("\nSocket created!\n");

printf("\nEstablishing connection to Jason...\n");

//Establish connection with Jason
if(connect(connectionSocket,(struct sockaddr*)&clientAddr,sizeof(clientAddr))<0) 
{
perror("\n Error while trying to CONNECT");
exit(0);
}


printf("\nClient(Volta) has connected to Jason %s!\n",ip);

//Get Data to be sent from User
printf("\n Enter an Integer:");
scanf("%d",&dataToTransmit.i);
dataToTransmit.i=htonl(dataToTransmit.i);
printf("\n Enter a Character:");
scanf("%c",&dataToTransmit.c);
dataToTransmit.c=getchar();

printf("\n Enter a Float Value:");
scanf("%f",&dataToTransmit.f);
dataToTransmit.f=htonf(dataToTransmit.f);


//Sends Data to Jason on Port 5036

printf("\nClient(Volta) is sending data to Jason...\n");

if (send(connectionSocket,&dataToTransmit,sizeof(dataToTransmit),0) < 0)
{
	printf("Error sending data");
	error(0);
}

printf("\nData has successfully been sent!\n");

printf("\n\n Attempting to close connection to Jason...\n");

//After Data has been sent to Jason Close the connection
close(connectionSocket);


printf("\n Connection to Jason has been closed!\n");

return 0;
}

//Function that resolves IP address from the Hostname given
int hostname_to_ip(char * hostname , char* ip)
{
struct hostent *he;
struct in_addr **addr_list;
int i;

if ( (he = gethostbyname( hostname ) ) == NULL)
{
	// get the host info
	herror("gethostbyname");
	return 1;
}

addr_list = (struct in_addr **) he->h_addr_list;

for(i = 0; addr_list[i] != NULL; i++)
{
	//Return the first one;
	strcpy(ip , inet_ntoa(*addr_list[i]) );
	return 0;
}
	return 1;
}

//Converts float in to network type
float htonf(float host)
{
float network;
unsigned char *h = (unsigned char *)&host;
unsigned char *n = (unsigned char *)&network;
n[0] = h[3];
n[1] = h[2];
n[2] = h[1];
n[3] = h[0];
return network;
}