#ifndef SOCKET_H
#define SOCKET_H

#include <map>
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
#include "MovieDb.h"
#include "Definitions.h"

class Socket
{
    public:
    // Constructor
    Socket( void );

    // Destructor
    ~Socket( void );

    int Listen();
    int SetupMedia();

//    enum QUERY{ QUERY_TITLE, QUERY_UNKNOWN};

    protected:

    private:
    int CreateSocket();
	void SendReply(const std::string &inStr);
	int CheckQuery(std::string inQuery);
    int mServerSock;
    int mClientSock;
    int mNewSock;
    int mPort;
	struct sockaddr_in mServer;
	struct sockaddr_in mClient;
	fd_set mReadfds;
	int mMaxSd;
	int mSd;
	int mMaxClients;
	int mClientSocket[2];
	int mActivity;

	MovieDb *mMdb;
//    Media *mMedia;

};  

#endif // Socket_H
