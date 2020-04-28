#include "Lcd.h"

const char* LCD_CHECKOUT_OUT_MESSAGE = "Check-Out";
const char* LCD_CHECKOUT_IN_MESSAGE = "Check-In";
const char* LCD_SCAN_CARD_MESSAGE = "Scan Card Now";
const char* LCD_SCAN_MOVIE_MESSAGE = "Scan Movie Now";
const char* LCD_CANCEL_MESSAGE = "Cancel";
const char* LCD_CHECK_OUT_SUC_MESSAGE = "Checked Out";
const char* LCD_CHECK_IN_SUC_MESSAGE = "Checked In";
const char* LCD_CHECK_OUT_ERR_MESSAGE = "Check Out Err";
const char* LCD_CHECK_IN_ERR_MESSAGE = "Check In Err";

void Lcd::Init()
{
	
    // Set LED BL
    bcm2835_gpio_fsel(LCD_BL, BCM2835_GPIO_FSEL_OUTP);
    // Set LED RS
    bcm2835_gpio_fsel(LCD_RS, BCM2835_GPIO_FSEL_OUTP);
    // Set LED E
    bcm2835_gpio_fsel(LCD_E, BCM2835_GPIO_FSEL_OUTP);
    // Set LED D4
    bcm2835_gpio_fsel(LCD_D4, BCM2835_GPIO_FSEL_OUTP);
    // Set LED D5
    bcm2835_gpio_fsel(LCD_D5, BCM2835_GPIO_FSEL_OUTP);
    // Set LED D6
    bcm2835_gpio_fsel(LCD_D6, BCM2835_GPIO_FSEL_OUTP);
    // Set LED D7
    bcm2835_gpio_fsel(LCD_D7, BCM2835_GPIO_FSEL_OUTP);

    FourBitInit();
    ClearDisplay();
    DisableLineWrap();

    char command = LCD_DISPLAY_SET;         // display set
    command |= LCD_DISPLAY_SET_ON;          // display ON
    command |= LCD_DISPLAY_SET_CURSOR_ON;   // cursor ON
    command |= LCD_DISPLAY_SET_BLINK_OFF;   // blink OFF
    SendCmd(command);

    command = LCD_FUNCTION_SET;             // function set
    command |= LCD_FUNCTION_DATA_LEN_4;     // 4 bit length
    command |= LCD_FUNCTION_2_LINE;         // 2 lines
    SendCmd(command);

    command = LCD_ENTRY_MODE_SET;           // Entry Mode Set
    command |= LCD_ENTRY_MODE_SET_INC;      // Increment
    command |= LCD_ENTRY_MODE_SET_NO_SHIFT; // No Shift
    SendCmd(command);

    ReturnHome();
    delay(2); 
    bcm2835_gpio_write(LCD_BL, LOW);
}

void Lcd::PulseEnable()
{
    delay(0.5); 
    bcm2835_gpio_write (LCD_E, HIGH); 
    delay(0.5); 
    bcm2835_gpio_write (LCD_E, LOW); 
    delay(0.5); 
}

void Lcd::Lcd_Byte(char bits)
{
	ClearBits();

	if ( bits & 0x10 )
	{
    	bcm2835_gpio_write(LCD_D4,HIGH);
	}
	if ( bits & 0x20 )
	{
    	bcm2835_gpio_write(LCD_D5,HIGH);
	}
	if ( bits & 0x40 )
	{
    	bcm2835_gpio_write(LCD_D6,HIGH);
	}
	if ( bits & 0x80 )
	{
    	bcm2835_gpio_write(LCD_D7,HIGH); 
	}

    PulseEnable();

	ClearBits();
	if ( bits & 0x1 )
	{
    	bcm2835_gpio_write(LCD_D4,HIGH);
	}
	if ( bits & 0x2 )
	{
    	bcm2835_gpio_write(LCD_D5,HIGH);
	}
	if ( bits & 0x4 )
	{
    	bcm2835_gpio_write(LCD_D6,HIGH);
	}
	if ( bits & 0x8 )
	{
    	bcm2835_gpio_write(LCD_D7,HIGH); 
	}
    PulseEnable();

}

void Lcd::SendCmd( char cmd ) 
{
    bcm2835_gpio_write (LCD_RS, LOW) ; 
    Lcd_Byte(cmd);
    delay(2); 
}

short Lcd::GetSize(char *pIn)
{
    short j=0;
    while ( *(pIn) !='\0')
    {
        pIn++;
        j++;
    }
    return j;
}

void Lcd::SendDataNoArrow(char *s) 
{
    //short size = GetSize(s);
    mLine1Buffer = s;
    
    bcm2835_gpio_write (LCD_RS, HIGH) ; 
    if ( false == GetLineWrap() )
    {
        for(short i=0; i<40; ++i)
        {
            if ( ( (int)*s >= 32 ) && ( (int)*s <=127 ) )
            {
    	        Lcd_Byte(*s++);
            }
        }

    }
    else
    {
        while(*s)
        {
            if ( ( (int)*s >= 32 ) && ( (int)*s <=127 ) )
            {
    	        Lcd_Byte(*s++);
            }
        }
    }

}

void Lcd::SendData(char *s, char inLine) 
{
    bcm2835_gpio_write (LCD_RS, HIGH) ; 
	if ( ! inLine ^ mArrowLoc ) // arrow is on this line
	{
    	SendSpecChar(0x7E); // arrow 
    	SendSpecChar(0x10); // space
	}
	else // arrow not on this line
	{
    	SendSpecChar(0x10); // space
    	SendSpecChar(0x10); // space
	}
    while(*s)
    {
        if ( ( (int)*s >= 32 ) && ( (int)*s <=127 ) )
        {
	        Lcd_Byte(*s++);
        }
    }
}

void Lcd::SendCancel(char inLine, bool inMovArr) 
{
    bcm2835_gpio_write (LCD_RS, HIGH) ; 
	if ( 0x0 == inLine ) // put cancel on line 1
	{
        SendCmd(LCD_CMD_BEG_LINE_1); // Set Cursor to beginning of line 1
        if ( inMovArr )
        {
	        mArrowLoc=0x0;
        }
	    SendData((char*)LCD_CANCEL_MESSAGE,0x0);
	}
	else // put cancel on line 2
	{
        SendCmd(LCD_CMD_BEG_LINE_2); // Set Cursor to beginning of line 2
        if ( inMovArr )
        {
	        mArrowLoc=0x1;
        }
	    SendData((char*)LCD_CANCEL_MESSAGE,0x1);
	}
}

void Lcd::SendSpecChar(char bits) 
{
	bcm2835_gpio_write (LCD_RS, HIGH) ; 
	Lcd_Byte(bits);
}

void Lcd::ClearBits()
{
	bcm2835_gpio_write(LCD_D4,LOW);
	bcm2835_gpio_write(LCD_D5,LOW);
	bcm2835_gpio_write(LCD_D6,LOW);
	bcm2835_gpio_write(LCD_D7,LOW);
}

void Lcd::InitialDisplay()
{
	mArrowLoc=0x0;
	bcm2835_gpio_write(LCD_BL, HIGH);
    ClearDisplay();
	SendData((char*)LCD_CHECKOUT_OUT_MESSAGE,0x0);
    SendCmd(0xC0);	// Next line
	SendData((char*)LCD_CHECKOUT_IN_MESSAGE,0x1);
    ReturnHome();
}

void Lcd::WriteLcd(const CHECK_IO &rcheck)
{
	mArrowLoc=0x0; // set arrown to first line
   	ClearDisplay();
    switch ( rcheck )
    {
        case MOVIE_CHECK_IO:
        {
		    SendDataNoArrow((char*)LCD_SCAN_MOVIE_MESSAGE);
            break;
        }
        case PERSON_CHECK_IO:
        {
		    SendDataNoArrow((char*)LCD_SCAN_CARD_MESSAGE);
            break;
        }
        case MOVIE_OUT_IO:
        {
		    SendDataNoArrow((char*)LCD_CHECK_OUT_SUC_MESSAGE);
            break;
        }
        case MOVIE_OUT_ERR_IO:
        {
		    SendDataNoArrow((char*)LCD_CHECK_OUT_ERR_MESSAGE);
            break;
        }
        case MOVIE_IN_IO:
        {
		    SendDataNoArrow((char*)LCD_CHECK_IN_SUC_MESSAGE);
            break;
        }
        case MOVIE_IN_ERR_IO:
        {
		    SendDataNoArrow((char*)LCD_CHECK_IN_ERR_MESSAGE);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Lcd::ArrowUp()
{
    if ( 1 == mArrowLoc ) // arrow on second line
    {
        if ( GetLcdMutex() )
        {
    		mArrowLoc=0x0; // set arrown to first line
            SendCmd(LCD_CMD_BEG_LINE_2); // Set Cursor to beginning of line 2
        	SendSpecChar(LCD_DATA_SPACE); // space
            SendCmd(LCD_CMD_BEG_LINE_1); // Set Cursor to beginning of line 1
        	SendSpecChar(LCD_DATA_ARROW); // arrow 
            RelLcdMutex();
    	}
        else // Set arrow location for LCD user to write
        {
    		mArrowLoc=0x0; // set arrown to first line
        }
    }
	// else do nothing
}

void Lcd::ArrowDown()
{
	if ( 0 == mArrowLoc ) // arrow on first line
	{
        if ( GetLcdMutex() )
        {
		    mArrowLoc=0x1; // set arrown to second line
            SendCmd(LCD_CMD_BEG_LINE_1); // Set Cursor to beginning of line 1
        	SendSpecChar(LCD_DATA_SPACE); // space
            SendCmd(LCD_CMD_BEG_LINE_2); // Set Cursor to beginning of line 2
        	SendSpecChar(LCD_DATA_ARROW); // arrow 
            RelLcdMutex();
        }
        else // Set arrow location for LCD user to write
        {
		    mArrowLoc=0x1; // set arrown to second line
        }
	}
	// else do nothing

}

bool Lcd::GetLcdMutex()
{
    if ( false == mLcdMutex )
    {
        mLcdMutex = true;
        return true;
    }
    return false;

}

bool Lcd::RelLcdMutex()
{
    if ( true == mLcdMutex )
    {
        mLcdMutex = false;
        return true;
    }
    return false;

}

void Lcd::FourBitInit()
{
    SendCmd(LCD_FUNCTION_SET);
    SendCmd(LCD_FUNCTION_SET);
    delay(2); 
}

void Lcd::ClearDisplay()
{
    SendCmd(LCD_CLEAR_DISPLAY); 
    delay(2); 
}

void Lcd::ReturnHome()
{
    SendCmd(LCD_RETURN_HOME); 
    delay(2); 
}

void Lcd::ScrollLeft(const short cDelay, char* s, char inLine, char cArrow)
{
    SendData(s,0x0);
    SendCancel(0x1,false);
    sleep(cDelay);
    // If arrow location on first line
    if ( 0x0 == mArrowLoc )
    {
        std::string tmp = s;
        short size = tmp.size();
        if ( size > 14 )
        {
            for ( int i=1; i< size;++i)
            {
                if ( 1 == bcm2835_gpio_lev(PIN_START_BUTTON) )
                {
                ClearDisplay();
                ReturnHome();
                // shift text 1
                SendData((char*)tmp.substr(i).c_str(),inLine);
                SendCancel(0x1,false);
                sleep(1);
                }
                else
                {
                    return;
                }
            }
        }
        ClearDisplay();
        ReturnHome();
        SendData(s,0x0);
        SendCancel(0x1,false);
    }
    else
    {
        char command = LCD_CUR_DISP_CONTROL;       //Cursor-Display Control
        command |= LCD_CUR_DISP_DISPLAY_SHIFT;     //Display Shifts
        command |= LCD_CUR_DISP_SHIFT_LEFT;        //Shift Left
        for (int i=0; i<40; i++)
        {
            SendCmd(command);
            sleep(1);
        }
    }
}

void Lcd::ScrollRight(const short cDelay, char* s, char inLine, char cArrow)
{
    SendData(s,0x0);
    sleep(cDelay);
    char command = LCD_CUR_DISP_CONTROL;       //Cursor-Display Control
    command |= LCD_CUR_DISP_DISPLAY_SHIFT;     //Display Shifts
    command |= LCD_CUR_DISP_SHIFT_RIGHT;       //Shift Right
    for (int i=0; i<40; i++)
    {
        SendCmd(command);
    	SendSpecChar(0x7E); // arrow 
    	SendSpecChar(0x10); // space
        sleep(1);
    }
}



