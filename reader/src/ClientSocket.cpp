#include "Controls.h"
#include "ClientSocket.h"
#include "Definitions.h"

int ClientSocket::SocketInit()
{
    if ((mClientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    mServSddr.sin_family = AF_INET; 
    mServSddr.sin_port = htons(50001); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &mServSddr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(mClientSock, (struct sockaddr *)&mServSddr, sizeof(mServSddr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }

	std::string reply = Receive();
	if ( "Movies Daemon" != reply )
	{
		return 1;
	}
 
    return 0;
};

int ClientSocket::Send(MEDIA_COMMAND_TYPE inQ, const std::string inStr )
{
	char buffer[1025];
	buffer[0] = inQ;
    char *p = buffer+1;
	strcpy(p,inStr.c_str());
    send(mClientSock , buffer, strlen(buffer) , 0 ); 
    return 0;
}

std::string ClientSocket::Receive()
{
    char buffer[1024] = {0}; 
    int value = read( mClientSock , buffer, 1024);
    buffer[value]= '\0';
    return buffer;

}
