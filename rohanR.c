#include<sys/socket.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#define SER_PORT 5020

float float_swap(float);

struct data 
{
        unsigned int i;
        char c;
        float f;
};

int main()
{

int sersock,receiveSocket;
struct data dataReceive;
struct sockaddr_in seraddr;
struct sockaddr_in cliinfo;

socklen_t csize=sizeof(cliinfo);
seraddr.sin_family=AF_INET;
seraddr.sin_port=htons(SER_PORT);
seraddr.sin_addr.s_addr=htonl(INADDR_ANY);

printf("\nCreating a socket...\n");

if((sersock=socket(AF_INET,SOCK_STREAM,0))<0) // creates socket
{
        perror("\n socket");
        exit(0);
}

printf("\nSocket has been created!\n");

printf("\nAttempting to bind the address to the socket...\n");


if(bind(sersock,(struct sockaddr *)&seraddr,sizeof(seraddr))<0)
{
        perror("\nBIND");
        exit(0);
}

printf("\nBind successful!\n");

printf("\nAttempting to listen...\n");


if(listen(sersock,1)<0) //listens
{
	printf("\nFailed to listen\n Exiting now...\n");
	exit(0);
        
}

printf("\n Listening...\n");


//Accepts Data From Jason
if((receiveSocket=accept(sersock,(struct sockaddr *)&cliinfo,&csize))<0)
{
perror("\n ACCEPT");
exit(0);
}
else
printf("\n Jason is now connected to Volta %s\n", inet_ntoa(cliinfo.sin_addr));

//Receives Data From Jason

//
if (recv(receiveSocket,&dataReceive,sizeof(dataReceive),0)<0)
{
	printf("Error receiving data");
	exit(0);
}

dataReceive.i=ntohl(dataReceive.i);
dataReceive.f = float_swap(dataReceive.f);

printf("\n Data Received:\n Integer :%u \n",(int)dataReceive.i);
printf("\n Character : %c\n",dataReceive.c);
printf("\n Float : %f\n",dataReceive.f);

printf("\nFinished receiving data!\n");
printf("\nAttempting to close connection...\n");


close(receiveSocket);
close(sersock);
printf("\n Connection Terminated! \n ");
return 0;
}

//Changes endianess - ref: cboardprogramming.com
float float_swap(float value){
    union v {
        float       f;
        unsigned int    i;
    };
     
    union v val;
     
    val.f = value;
    val.i = htonl(val.i);
                   
    return val.f;
}