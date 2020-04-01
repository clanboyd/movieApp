#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <iostream>
#include <vector>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <bcm2835.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class MySerial
{
    public:

    // Constructor
    MySerial();
    // Destructor
    ~MySerial();

    static MySerial *GetMySerial();
    static void RelMySerial();
    void SetSocket(int* sock);
    int* GetSocket();
    bool Open();
    bool Close();
    bool Read( std::string &rResult );
    bool GetRfIdMutex();
    bool RelRfIdMutex();

protected:
    int *mFd;

private:
    static MySerial *mSerial;
    bool mRfIdMutex;
};

#endif // MYSERIAL_H

