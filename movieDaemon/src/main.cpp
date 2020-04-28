#include "MovieDaemon.h"

int main()
{
    Socket *mySocket = new Socket();
	mySocket->InitDb();
	mySocket->Listen();
    delete mySocket;
    return 0;

}
