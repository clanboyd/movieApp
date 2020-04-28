#include "Lcd.h"
#include <iostream>
#include <stdio.h>
Lcd *gLcd;

#define LCD_BL RPI_V2_GPIO_P1_07

int Initialize()
{
    if ( !bcm2835_init() )
    {
        return 1;
    }
    // Set LED BL  
    bcm2835_gpio_fsel(LCD_BL, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(LCD_BL, HIGH);
    return 0;
}

void Unitialize()
{
    bcm2835_close();
}

int main()
{
    if ( 1 == Initialize() )
    {
        return 1;
    }

    gLcd = new Lcd();

    char command = LCD_FUNCTION_SET;        // function set  for 4-BIT
    printf("Sending Function Set 4 BIT: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    printf("Sending Function Set 4 BIT AGAIN: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    command = LCD_CLEAR_DISPLAY;            // clear display
    printf("Sending Clear Display: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    command = LCD_DISPLAY_SET;              // display set
    command |= LCD_DISPLAY_SET_ON;          // display ON
    command |= LCD_DISPLAY_SET_CURSOR_ON;   // cursor ON
    command |= LCD_DISPLAY_SET_BLINK_OFF;   // blink OFF
    printf("Sending Display ON, Cursor ON, Blink OFF: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    command = LCD_FUNCTION_SET;             // function set 
    command |= LCD_FUNCTION_DATA_LEN_4;     // 4 bit length
    command |= LCD_FUNCTION_2_LINE;         // 2 lines
    printf("Sending 8 bit length, 2 lines: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    command = LCD_ENTRY_MODE_SET;           // Entry Mode Set 
    command |= LCD_ENTRY_MODE_SET_INC;      // Increment
    command |= LCD_ENTRY_MODE_SET_NO_SHIFT; // No Shift
    printf("Increment by 1, No Shift: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    command = LCD_RETURN_HOME;             // Return Home
    printf("Return Home: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    printf("Init Complete\n");

    sleep(.5);

    command = LCD_DISPLAY_SET;              // display set
    command |= LCD_DISPLAY_SET_ON;          // display ON
    command |= LCD_DISPLAY_SET_CURSOR_ON;   // cursor ON
    command |= LCD_DISPLAY_SET_BLINK_ON;    // blink ON
    printf("Blink ON: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    sleep(.5);
    gLcd->EnableLineWrap();

    printf("Sending text to wrap\n");
    gLcd->SendDataNoArrow((char*)"A very long line for testing of the memory banks");

    sleep(1);
    gLcd->ClearDisplay();
    gLcd->DisableLineWrap();

    printf("Sending text no wrap\n");
    gLcd->SendDataNoArrow((char*)"A very long line for testing of the memory banks");

    sleep(1);

    command = LCD_DDRAM_SET;              //Set DDRAM Address
    command |= 0x0;                       //Address 0x0
    printf("Set cursor to beginning of line 1.\n");
    gLcd->SendCmd(command);

    sleep(1);

    printf("Shift Left, scrolling to Right.\n");
    gLcd->ScrollRight(0,(char*)"A very long line for testing of the memory banks",0x1,0x0);

    printf("Shift Right, scrolling to left.\n");
    gLcd->ScrollLeft(0,(char*)"A very long line for testing of the memory banks",0x1,0x0);

    printf("Return Home: 0x%02hhx\n", command);
    gLcd->SendCmd(command);

    Unitialize();

    delete gLcd;
    return 0;
};
