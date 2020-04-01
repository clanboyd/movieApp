#include "Lcd.h"

const char* LCD_CHECKOUT_OUT_MESSAGE = "Check-Out";
const char* LCD_CHECKOUT_IN_MESSAGE = "Check-In";
const char* LCD_SCAN_CARD_MESSAGE = "Scan Card Now";
const char* LCD_SCAN_MOVIE_MESSAGE = "Scan Movie Now";
const char* LCD_CANCEL_MESSAGE = "Cancel";

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

    SendCmd(LCD_CMD_INIT);
    SendCmd(LCD_CMD_4_BIT);
    SendCmd(0x06); // Increment Address by 1, Shift cursor to right
    SendCmd(0x0E); // Display ON, Cursor ON, Cursor Blinking
    SendCmd(0x28); 
    SendCmd(LCD_CMD_RET_HOME); // Return to home
	ClearDisplay();
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
    delay(0.5); 
}

void Lcd::SendDataNoArrow(char *s) 
{
    bcm2835_gpio_write (LCD_RS, HIGH) ; 
    while(*s)
    {
	    Lcd_Byte(*s++);
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
	    Lcd_Byte(*s++);
    }
}

void Lcd::SendCancel(char inLine) 
{
    bcm2835_gpio_write (LCD_RS, HIGH) ; 
	if ( 0x0 == inLine ) // put cancel on line 1
	{
	    mArrowLoc=0x0;
        SendCmd(LCD_CMD_BEG_LINE_1); // Set Cursor to beginning of line 1
	    SendData((char*)LCD_CANCEL_MESSAGE,0x0);
	}
	else // put cancel on line 2
	{
	    mArrowLoc=0x1;
        SendCmd(LCD_CMD_BEG_LINE_2); // Set Cursor to beginning of line 2
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

void Lcd::ClearDisplay()
{
    SendCmd(0x01); 
    delay(2); 
}

void Lcd::InitialDisplay()
{
	mArrowLoc=0x0;
	bcm2835_gpio_write(LCD_BL, HIGH);
    ClearDisplay();
	SendData((char*)LCD_CHECKOUT_OUT_MESSAGE,0x0);
    SendCmd(0xC0);	// Next line
	SendData((char*)LCD_CHECKOUT_IN_MESSAGE,0x1);
    SendCmd(0x02);	// Return Home
}

void Lcd::CheckInOut(char s)
{
	mArrowLoc=0x0; // set arrown to first line
   	ClearDisplay();
	if ( 0x0 == s )
	{
		SendDataNoArrow((char*)LCD_SCAN_MOVIE_MESSAGE);
	}
	else
	{
		SendDataNoArrow((char*)LCD_SCAN_CARD_MESSAGE);
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



