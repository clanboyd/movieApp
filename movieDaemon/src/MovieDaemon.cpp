#include "MovieDaemon.h"

Socket::Socket( void )
{
    mMdb=NULL;
    mPort=50001; 
	CreateSocket();
	mMdb = new MovieDb();
};

Socket::~Socket( void )
{
    if ( NULL != mMdb )
    {
        delete mMdb;
    }
};

int Socket::CreateSocket()
{
    int opt=1;
	mMaxClients=2;
	int i=0;

    for (i = 0; i < mMaxClients; i++)   
    {   
        mClientSocket[i] = 0;   
    }   

    mServerSock = socket( AF_INET, SOCK_STREAM, 0 );
    if( 0 > mServerSock )
    {
        printf("Socket Creation Failed: %s.", strerror(errno));
        close(mServerSock);
        return 1;
    }

    memset(&mService, 0, sizeof(mService));
    mService.sin_family      = AF_INET;
    mService.sin_addr.s_addr = INADDR_ANY;
    mService.sin_port        = htons(mPort);

    if ( setsockopt(mServerSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        printf("Set Socket Option Failed: %s.", strerror(errno));
        close(mServerSock);
        return 1;
    }

    if (bind(mServerSock, (struct sockaddr *)&mService,  sizeof(mService))<0) 
    { 
        printf("Socket Bind Failed: %s.", strerror(errno));
        close(mServerSock);
        return 1;
    }
 
    if (listen(mServerSock, 2) < 0) 
    { 
        printf("Socket Listen Failed: %s.", strerror(errno));
        close(mServerSock);
        return 1;
    }

    return 0;
}

int Socket::Listen()
{
    int addrlen = sizeof(mService);
	int i=0;
	unsigned int newSocket;
	char *message = (char*)"Movies Daemon"; 
	char buffer[1025];  //data buffer of 1K
    int valread=0;

	while (1)
	{
		FD_ZERO(&mReadfds);
		FD_SET(mServerSock, &mReadfds);
		mMaxSd=mServerSock;

	 	//add child sockets to set
        for ( i = 0 ; i < mMaxClients ; i++)
        {
        	//socket descriptor
            mSd = mClientSocket[i];

            //if valid socket descriptor then add to read list
            if(mSd > 0)
			{
                FD_SET( mSd, &mReadfds);
			}
                 
            //highest file descriptor number, need it for the select function
            if(mSd > mMaxSd)
			{
                mMaxSd = mSd;
			}
        }

		//wait for an activity on one of the sockets , timeout is NULL,
        // so wait indefinitely
		mActivity = select( mMaxSd + 1 , &mReadfds, NULL , NULL , NULL);

        if ((mActivity < 0) && (errno!=EINTR))
        {   
            printf("select error");
        }
 
        //If something happened on the master socket, //then its an incoming connection 
        if (FD_ISSET(mServerSock, &mReadfds))
        {
            if ((newSocket = accept(mServerSock,
                    (struct sockaddr *)&mService, (socklen_t*)&mService))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n", \
                newSocket, inet_ntoa(mService.sin_addr) , ntohs (mService.sin_port));

            //send new connection greeting message
            if( (unsigned int)send(newSocket, message, strlen(message), 0) 
                != (unsigned int)strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < mMaxClients; i++)
            {
                //if position is empty
                if( mClientSocket[i] == 0 )
                {
                    mClientSocket[i] = newSocket;
                    printf("Adding to list of sockets as %d\n" , i);
                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < mMaxClients; i++)
        {
            mSd = mClientSocket[i];

            if (FD_ISSET( mSd , &mReadfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                if ((valread = read(mSd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(mSd , (struct sockaddr*)&mService, \
                        (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                          inet_ntoa(mService.sin_addr) , ntohs(mService.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( mSd );
                    mClientSocket[i] = 0;
                }
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    printf("Received: %s\n", buffer);
					// determine query type and return answer
					CheckQuery(buffer);
                }
            }
        }
    }

    return 0;
	
}

int Socket::CheckQuery(std::string inQuery)
{
	switch ( MEDIA_COMMAND_TYPE(inQuery[0]) )
	{
		case MEDIA_OUT:
		{
			SendReply(mMdb->FindTitleById(inQuery.substr(1)));
			break;
		}
		default:
		{
			break;
		}
	}
    return 0;
}

void Socket::SendReply(const std::string &inStr)
{
	send(mSd, inStr.c_str(), strlen(inStr.c_str()) , 0 );
}
