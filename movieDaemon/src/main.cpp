#include "MovieDaemon.h"

int main()
{
    Socket *mySocket = new Socket();
	mySocket->Listen();
    delete mySocket;
    return 0;

}
