#ifndef CONTROLS_H
#define CONTROLS_H

#include "Definitions.h"
#include "Lcd.h"
#include "ClientSocket.h"
#include "MySerial.h"



class Controls
{
    public:

    // Constructor
    Controls();
    // Destructor
    ~Controls(){};
   
    bool Init(); 
    void DeInit(); 
    static void* Start(void* obj);
    void Start(); 
    static void* PerformAction(void* obj);
    void PerformAction(); 
    static void* CheckUpButton(void* obj);
    void CheckUpButton(); 
    static void* CheckDownButton(void* obj);
    void CheckDownButton(); 
    static void* GreenLedBlink(void* obj);
    void GreenLedBlink(); 
    void SendToLcd(const std::string &rInStr); 
    void SendErrorToLcd(const std::string &rInStr); 
    void PerformCheckOut(); 
    void PerformCheckIn(); 
    void Read(); 

protected:

private:
    MySerial *mSerPointer;
    ClientSocket *mClientSock;
    
    Lcd *mLcd;
	enum Stage{ STARTUP, CHECKOUT, CHECKIN, READNAME, READMOVIE, UNKNOWN};
	std::stack<Stage> mStageStack;
};

#endif // CONTROLS_H

