#include<sys/socket.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h> //memset
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define SER_PORT 5020
#define SER_PORT1 5020

//Holds dataBeforeBuffer to be sent back to Volta
struct dataBeforeBuffer
{
        int i;
        char c;
        float f;
};

float htonf(float host); //Converts float for network transmission
int hostname_to_ip(char * , char *); //resolves ip address

int main(int argc , char *argv[])
{

if(argc <2)
{
        printf("Please provide a hostname to resolve");
        exit(1);
}

int r = 0;
int serverSocket,receiveSocket;
struct dataBeforeBuffer dataBefore;
struct sockaddr_in serverAddr;
struct sockaddr_in clientInfo;

socklen_t csize=sizeof(clientInfo);
serverAddr.sin_family=AF_INET;
serverAddr.sin_port=htons(SER_PORT);
serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);

printf("\nCreating a socket...\n");

//Creates Socket
if((serverSocket=socket(AF_INET,SOCK_STREAM,0))<0)
{
        perror("\n socket");
        exit(0);
}

printf("\nSocket has been created!\n");

printf("\nAttempting to bind the address to the socket...\n");


//Binds Socket
if(bind(serverSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)
{
        perror("\nBIND");
        exit(0);
}

printf("\nBind successful!\n");

printf("\nAttempting to listen...\n");


//Listens for Volta's Request
if(listen(serverSocket,1)<0)
{
perror("\n LISTEN");
}


//Jason is waiting for Volta to try and connect
printf("\n Listening...");

//Accepts Volta's dataBeforeBuffer
if((receiveSocket=accept(serverSocket,(struct sockaddr *)&clientInfo,&csize))<0) 
{
perror("\n ACCEPT ");
exit(0);
}
else //Jason has excepted request from Volta to connect
printf("\nJason is now connected to %s(Volta)\n",inet_ntoa(clientInfo.sin_addr));

//Receives dataBeforeBuffer Sent from Volta
if (recv(receiveSocket,&dataBefore,sizeof(dataBefore),0) < 0)
{
		perror("Error receiving data");
}

dataBefore.i=ntohl(dataBefore.i);

printf("\n --Before Changing Data-- \n Data Received:  \n Integer:%d \n",(int)dataBefore.i);
printf("\n Character:%c \n ",dataBefore.c);
printf("\n Float:%f \n",(float)dataBefore.f);

printf("\nFinished receiving data!\n");

printf("\nAttempting to close connection...\n");

printf("\n Connection Terminated! \n ");


// client code
char *hostname = argv[1];
char ip[100];
hostname_to_ip(hostname , ip); // resolves ip address
struct dataBeforeBuffer dataBeforeAfter;
int clientSocket;
struct sockaddr_in cliaddr;
cliaddr.sin_port=htons(SER_PORT1);
cliaddr.sin_family=AF_INET;
cliaddr.sin_addr.s_addr=inet_addr(ip);

printf("\n Press Enter to connect and send data back to Volta");
getchar();

printf(" \n%s resolved to %s\n" , hostname , ip);


//Creates new Socket to connect back to Volta

printf("Creating a socket...");

clientSocket=socket(AF_INET,SOCK_STREAM,0);

if(clientSocket<0)
{
perror("\n SOCKET");
exit(0);
}

printf("\nSocket created!\n");


printf("\nEstablishing connection to Volta...\n");

//Establishes new connection to Volta to send changed data back
if(connect(clientSocket,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
{
perror("\n CONNECT");
exit(0);
}

//Jason has established a connection to Volta for transmission
printf("\n Client(Jason) has connected to (Volta)%s",ip);

//Multiply Integer value by 2
dataBeforeAfter.i=dataBefore.i*2;

//Find next letter in alphabet
if(((int)dataBefore.c>=65 && (int)dataBefore.c<90) || ((int)dataBefore.c>=97 &&
(int)dataBefore.c<122))
{
dataBeforeAfter.c=dataBefore.c+1;
}
if(((int)dataBefore.c==90) || ((int)dataBefore.c==122))
{
dataBeforeAfter.c=dataBefore.c-25;
}

//Add 1 to float value
dataBeforeAfter.f=dataBefore.f+1;
printf("\n --After Changing Data-- \n After: \n Integer:%d\n",(int)dataBeforeAfter.i);
printf("\n Character: %c\n",dataBeforeAfter.c);
printf("\n Float: %f\n",(float)dataBeforeAfter.f);

printf("\nClient(Jason) is sending data to Server(Volta)...\n");


//Send dataBefore Back to Volta on new Connection

if (send(clientSocket,&dataBeforeAfter,sizeof(dataBeforeAfter),0) < 0)
	printf("\n Integer dataBefore could not be sent");


printf("\nData has successfully been sent!\n");

printf("\n\n Attempting to close connection to Volta...\n");


//Close Connection to Volta
close(receiveSocket);
close(serverSocket);
printf("\n\n Connection Terminated! \n");
return 0;
}


//Gets host IP from addr_list
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