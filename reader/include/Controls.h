#ifndef CONTROLS_H
#define CONTROLS_H

#include "Definitions.h"
#include "Lcd.h"
#include "ClientSocket.h"
#include "MySerial.h"



class Controls
{
    public:

    struct PerformArgs                                                                                                                     
    {                                                                                                                                      
        Controls *obj;                                                                                                                     
        std::string error;
        std::string movieid;
        std::string personid;
        std::string moviename;
        std::string personname;
        USER_RESULT result;
        MEDIA_COMMAND_TYPE type;                                                                                                           
    };     

    // Constructor
    Controls();
    // Destructor
    ~Controls(){};
   
    bool Init(); 
    void DeInit(); 
    static void* Start(void* obj);
    void Start(); 
    static void *PerformAction(void* obj); 
    void PerformAction(PerformArgs *pArgsIn);
    static void* CheckUpButton(void* obj);
    void CheckUpButton(); 
    static void* CheckDownButton(void* obj);
    void CheckDownButton(); 
    static void* GreenLedBlink(void* obj);
    void GreenLedBlink(); 
    void SendToLcd(const std::string &rInStr); 
    void SendErrorToLcd(const std::string &rInStr); 
    void WriteLcd(const Lcd::CHECK_IO &rcheck); 
    void Read(PerformArgs *pArgsIn);
    void waitForEncoderTick(uint8_t pin);
    void PerformCheckOut(PerformArgs *pArgsIn);
    void PerformCheckIn(PerformArgs *pArgsIn);

protected:

private:
    MySerial *mSerPointer;
    ClientSocket *mClientSock;
    
    Lcd *mLcd;
	enum Stage{ STARTUP, CHECKOUT, CHECKIN, READNAME, READMOVIE, UNKNOWN};
	std::stack<Stage> mStageStack;
};

#endif // CONTROLS_H

