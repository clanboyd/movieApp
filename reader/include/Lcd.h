#ifndef LCD_H
#define LCD_H

#include "Definitions.h"
#include <vector>
#include <string>
#include <stack>

#define LCD_CMD_INIT 0x33
#define LCD_CMD_4_BIT 0x32
#define LCD_CMD_RET_HOME 0x02
#define LCD_CMD_BEG_LINE_1 0x80
#define LCD_CMD_BEG_LINE_2 0xC0
#define LCD_DATA_ARROW 0x7E
#define LCD_DATA_SPACE 0x20

class Lcd
{
    public:

    // Constructor
    Lcd(){};
    // Destructor
    ~Lcd(){};
    enum CHECK_IO
    { 
        MOVIE_CHECK_IO, 
        PERSON_CHECK_IO, 
        MOVIE_OUT_IO,
        MOVIE_OUT_ERR_IO, 
        MOVIE_IN_IO, 
        MOVIE_IN_ERR_IO, 
        IO_UNKNOWN
    };

    void Init();
    void PulseEnable();
    void Lcd_Byte(char bits);
    void SendCmd(char cmd);
    void SendData(char* s, char inLine);
    void SendCancel(char inLine, bool inMovArr = true);
    void SendDataNoArrow(char* s);
    void SendSpecChar(char bits);
    void ClearBits();
    void ClearDisplay();
	void InitialDisplay();
	void ArrowUp();
	void ArrowDown();
	char GetArrowLoc() { return mArrowLoc; }
	void WriteLcd(const CHECK_IO &rcheck);
	bool GetLcdMutex();
	bool RelLcdMutex();

protected:

private:
	char mArrowLoc;
    bool mLcdMutex; 

};

#endif // LCD_H

