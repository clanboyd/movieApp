#include "MySerial.h"    

MySerial *MySerial::mSerial = NULL;
const int PORT_SPEED=9600;

std::string gRaw;
static std::string gLastError;

MySerial::MySerial()
{
   mFd = NULL;
};

MySerial::~MySerial() {};

MySerial *MySerial::GetMySerial()
{
    if ( mSerial == NULL )
    {
        mSerial = new MySerial();
    }
    if ( false == mSerial->Open() )
    {
        delete mSerial;
        mSerial=NULL;
    }
    return mSerial;

} 

void MySerial::RelMySerial()
{
    if ( mSerial != NULL )
    {
        delete mSerial;
        mSerial = NULL;
    }
}

bool MySerial::Open()
{
    mFd = new int;
    if ( NULL == mFd )
    {
        return false;
    }
    *mFd = open ("/dev/ttyAMA0", O_RDWR|O_NOCTTY);
    if ( *mFd < 0 )
    {
        std::cout<<"Unable to open /dev/ttyAMA0, ERRNO= " << errno <<std::endl;
        return false;
    }
    struct termios com;
    tcgetattr(*mFd, &com);
	com.c_cflag &= ~PARENB;
	com.c_cflag &= ~CSTOPB;
    com.c_cflag |= B9600;
	com.c_cflag |= CSIZE;
    com.c_cflag |= CS8;
    com.c_cflag |= CREAD;
    com.c_cflag |= CLOCAL;
    com.c_iflag &= ~BRKINT;
    com.c_iflag &= ~IXOFF;
    com.c_iflag &= ~IXON;
    com.c_iflag &= ~ICRNL;
    com.c_iflag |= IGNBRK;
    com.c_oflag &= ~OPOST;
    com.c_oflag &= ~ONLCR;
    com.c_lflag &= ~ISIG;
    com.c_lflag &= ~ICANON;
    com.c_lflag &= ~IEXTEN;
    com.c_lflag &= ~ECHO;
    com.c_lflag &= ~ECHOE;
    com.c_lflag &= ~ECHOK;
    com.c_lflag &= ~ECHOKE;
    com.c_cc[VMIN] = 1;
    com.c_cc[VTIME] = 5; // timeout 25.5 sec
    tcsetattr(*mFd, TCSANOW, &com);

    return true;
}

bool MySerial::Close()
{
    close (*mFd);
    return true;
}

int* MySerial::GetSocket()
{
   return mFd;
}

void MySerial::SetSocket(int* sock)
{
   mFd = sock;
}

bool MySerial::Read( std::string &rResult )
{
    if ( GetRfIdMutex() )
    {
        char str[12];
        *str = 0;
        rResult = str; // clean the string
        int eol = 0;
        while(!eol)
        {
           int res = read(*mFd,str,12);
           if( res > 0 )
           {
              str[res] = 0;
              //std::cout<<"read returned " << res << " characters | string " << str << std::endl;
           }
           else // 0 means timeout
           {
                //std::cout<<"read returned " << res <<" -> FAILURE" << std::endl;
                return false;
           }
           for(int i=0;i<res;i++)
           {
              if(str[i]=='\n')
              {
                 eol = 1;
                 break;
              }
              else if(str[i]>=' ')
              {
                 rResult += str[i];
              }
           }
           //std::cout<<"Text: " << rResult << std::endl;
        }
        RelRfIdMutex();
        return true;
    }
    else
    {
        return false;
    }

    return false;
}  // End Read

bool MySerial::GetRfIdMutex()
{
    if ( false == mRfIdMutex )
    {
        mRfIdMutex = true;
        return true;
    }
    return false;

}

bool MySerial::RelRfIdMutex()
{
    if ( true == mRfIdMutex )
    {
        mRfIdMutex = false;
        return true;
    }
    return false;

}
