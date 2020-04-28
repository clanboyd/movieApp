#ifndef LCD_H
#define LCD_H

#include "Definitions.h"
#include <vector>
#include <string>
#include <stack>

//Clear Display
#define LCD_CLEAR_DISPLAY           0x01

//Return Home
#define LCD_RETURN_HOME             0x02

//Entry Mode Set
#define LCD_ENTRY_MODE_SET          0x04
#define LCD_ENTRY_MODE_SET_INC      0x02
#define LCD_ENTRY_MODE_SET_DEC      0x00
#define LCD_ENTRY_MODE_SET_SHIFT    0x01
#define LCD_ENTRY_MODE_SET_NO_SHIFT 0x00

//Display Control
#define LCD_DISPLAY_SET             0x08
#define LCD_DISPLAY_SET_ON          0x04
#define LCD_DISPLAY_SET_OFF         0x00
#define LCD_DISPLAY_SET_CURSOR_ON   0x02
#define LCD_DISPLAY_SET_CURSOR_OFF  0x00
#define LCD_DISPLAY_SET_BLINK_ON    0x01
#define LCD_DISPLAY_SET_BLINK_OFF   0x00

//Cursor-Display Control
#define LCD_CUR_DISP_CONTROL        0x10
#define LCD_CUR_DISP_DISPLAY_SHIFT  0x08
#define LCD_CUR_DISP_CURSOR_MOVE    0x00
#define LCD_CUR_DISP_SHIFT_RIGHT    0x04
#define LCD_CUR_DISP_SHIFT_LEFT     0x00

//Function Set
#define LCD_FUNCTION_SET            0x20
#define LCD_FUNCTION_DATA_LEN_8     0x10
#define LCD_FUNCTION_DATA_LEN_4     0x00
#define LCD_FUNCTION_2_LINE         0x08
#define LCD_FUNCTION_1_LINE         0x00
#define LCD_FUNCTION_FONT_5_BY_10   0x04
#define LCD_FUNCTION_FONT_5_BY_8    0x00

//Set DDRAM Address
#define LCD_DDRAM_SET               0x80

//Set CGRAM Address
#define LCD_CGRAM_SET               0x40

// Beginning of Lines
#define LCD_CMD_BEG_LINE_1 0x80
#define LCD_CMD_BEG_LINE_2 0xC0

#define LCD_DATA_ARROW 0x7E
#define LCD_DATA_SPACE 0x20

class Lcd
{
    public:

    // Constructor
    Lcd(){DisableLineWrap();};
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
	void InitialDisplay();
	void ArrowUp();
	void ArrowDown();
	char GetArrowLoc() { return mArrowLoc; }
	void WriteLcd(const CHECK_IO &rcheck);
	bool GetLcdMutex();
	bool RelLcdMutex();
	void EnableLineWrap(){mLineWrap=true;};
	void DisableLineWrap(){mLineWrap=false;};
	bool GetLineWrap(){return mLineWrap;};
    short GetSize(char *pIn);

    void FourBitInit();
    void ClearDisplay();
    void ReturnHome();
    void ScrollRight(const short cDelay, char* s, char inLine, char cArrow);
    void ScrollLeft(const short cDelay, char* s, char inLine, char cArrow);

protected:

private:
	char mArrowLoc;
    bool mLcdMutex; 
    bool mLineWrap; 
    std::string mLine1Buffer;
    std::string mLine2Buffer;

};

#endif // LCD_H

