#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>
#include <clocale>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

class ClientSocket
{
    public:
    // Constructor
    ClientSocket( void ){};

    // Destructor
    ~ClientSocket( void ){};

	int Send(MEDIA_COMMAND_TYPE inQ, const std::string inStr );
    int SocketInit();
    std::string Receive();
    protected:

    private:
    int mClientSock;
    struct sockaddr_in mServSddr;

};  

#endif // Socket_H

