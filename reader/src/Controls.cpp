#include "Controls.h"

struct PerformArgs
{     
    Controls *obj; 
    std::string error;
    std::string id;
    bool result;
    MEDIA_COMMAND_TYPE type;
};

Controls::Controls()
{
    // Get Serial Pointer
    mSerPointer = mSerPointer->GetMySerial();
}

bool Controls::Init()
{
    if ( !bcm2835_init() )
    {
        return false;
    }

    // Set RFID Reset
    bcm2835_gpio_fsel(PIN_RFID_RESET, BCM2835_GPIO_FSEL_OUTP);
    //  Set RFID Reset OFF
    bcm2835_gpio_write(PIN_RFID_RESET, LOW);

    // Set Start Button
    bcm2835_gpio_fsel(PIN_START_BUTTON, BCM2835_GPIO_FSEL_INPT);
    //  Set Start button as pullup
    bcm2835_gpio_set_pud(PIN_START_BUTTON, BCM2835_GPIO_PUD_UP);
    // And a low detect enable
    //bcm2835_gpio_len(PIN_START_BUTTON);

    // Set Down Button
    bcm2835_gpio_fsel(PIN_DOWN_BUTTON, BCM2835_GPIO_FSEL_INPT);
    //  Set Down button as pullup
    bcm2835_gpio_set_pud(PIN_DOWN_BUTTON, BCM2835_GPIO_PUD_UP);
    // And a low detect enable
    bcm2835_gpio_len(PIN_DOWN_BUTTON);

    // Set Up Button
    bcm2835_gpio_fsel(PIN_UP_BUTTON, BCM2835_GPIO_FSEL_INPT);
    //  Set Up button as pullup
    bcm2835_gpio_set_pud(PIN_UP_BUTTON, BCM2835_GPIO_PUD_UP);
    // And a low detect enable
    bcm2835_gpio_len(PIN_UP_BUTTON);

    // Set Green LED
    bcm2835_gpio_fsel(PIN_GREEN_LIGHT, BCM2835_GPIO_FSEL_OUTP);
    //  Set Green Light OFF
    bcm2835_gpio_write(PIN_GREEN_LIGHT, LOW);

    // Set Purple LED
    bcm2835_gpio_fsel(PIN_PURPLE_LIGHT, BCM2835_GPIO_FSEL_OUTP);
    //  Set Purple button OFF
    bcm2835_gpio_write(PIN_PURPLE_LIGHT, LOW);

    mLcd = new Lcd();
    mLcd->Init();

    mClientSock = new ClientSocket();
	if ( 1 == mClientSock->SocketInit() )
	{
    	return false;
	}

    return true;
}

void Controls::DeInit()
{
    bcm2835_close();
    delete mLcd;
    delete mClientSock;
}

void* Controls::Start(void* obj)
{
    ((Controls *) obj)->Start();
    return obj;
}

void Controls::Start()
{
    bool runPa = true;
    pthread_t action;
    std::string str;
    while (1)
    {
        if ( 0 == bcm2835_gpio_lev(PIN_START_BUTTON) )
        {
            sleep(1);
			std::cout<<"Button Pressed"<<std::endl;
			mLcd->InitialDisplay();
            bcm2835_gpio_write(PIN_GREEN_LIGHT, HIGH);
            while (runPa)
            {
                if ( 0 == bcm2835_gpio_lev(PIN_START_BUTTON) )
                {
                    // Proceed to CheckOut Movie
        			if ( 0x0 == mLcd->GetArrowLoc() )
        			{
                        if ( mLcd->GetLcdMutex() ) 
                        { 
                            PerformArgs myP;
                            myP.obj = this;
                            myP.error = UNK_MOVIE;
                            myP.type = MEDIA_MOVIE_FIND;
                            myP.result = MAX_USER;
                            myP.movieid = "0";
                            myP.personid = "0";
                            pthread_create(&action, NULL, &PerformAction,&myP);
                            pthread_join(action,NULL);
                            if ( myP.result != USER_CANCEL ) 
                            {
                                myP.error = UNK_PERSON;
                                myP.type = MEDIA_PERSON_FIND;
                                pthread_create(&action, NULL, &PerformAction,&myP);
                                pthread_join(action,NULL);
    
                                if ( ( myP.movieid != "0" && myP.personid != "0" ) && myP.result == USER_CONTINUE )
                                {
                                    myP.error = IO_SUCCESS;
                                    myP.type = MEDIA_OUT;
                                    pthread_create(&action, NULL, &PerformAction,&myP);
                                    pthread_join(action,NULL);
                                }
                            }
                        }
        			}
                    // Cancel
        			else
        			{
                        //if ( mLcd->GetLcdMutex() ) 
                        //{ 
            		//		PerformCheckInOut();
                    //        mLcd->RelLcdMutex();
                     //       runPa=false;
                      //  }
        			}
        			sleep(1); // allow current to go back to 1, read event not working in this kernel
        		}
            }
            // Go back to Initial Display
			mLcd->InitialDisplay();
        }
    }
};

void Controls::Read(PerformArgs *pArgsIn)
{
    bool runRead=true;
	std::string id;
    while ( runRead )
    {
        bcm2835_gpio_write(PIN_RFID_RESET, HIGH);
    	if ( true == mSerPointer->Read(id) )
    	{
			std::cout<<"Sending: "<<id<<std::endl;
	    	mClientSock->Send(pArgsIn->type, id); 
	    	std::string tmp = mClientSock->Receive(); 
            if ( tmp != pArgsIn->error )
            {
                SendToLcd(tmp);
                mLcd->RelLcdMutex(); // Allow up/down arrow
				sleep(1); 
                bcm2835_gpio_write(PIN_RFID_RESET, LOW);
                // Check if Cancel or Proceed
                while ( 1 == bcm2835_gpio_lev(PIN_START_BUTTON) ) {}
                // If Arrow Location is 0, then Proceed
                if ( 0x0 == mLcd->GetArrowLoc() )
                {
                    switch ( pArgsIn->type )
                    {
                        case MEDIA_MOVIE_FIND:
                        {
                            pArgsIn->movieid = id;
                            pArgsIn->moviename = tmp;
                            break;
                        }
                        case MEDIA_PERSON_FIND:
                        {
                            pArgsIn->personid = id;
                            pArgsIn->personname = tmp;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    pArgsIn->result = USER_CONTINUE;
                    runRead=false;
                }
                // If Arrow Location is 1, then Cancel
                else
                {
                    pArgsIn->result = USER_CANCEL;
                    runRead=false;
                }
            }
            else
            {
                SendErrorToLcd(tmp);
				sleep(1); 
                bcm2835_gpio_write(PIN_RFID_RESET, LOW);
                pArgsIn->result = USER_ERROR;
                // Wait for Cancel
                while ( 1 == bcm2835_gpio_lev(PIN_START_BUTTON) ) {}
                runRead=false;
            }
        }
	}
}

void Controls::PerformCheckOut(PerformArgs *pArgsIn)
{
	mClientSock->Send(pArgsIn->type, pArgsIn->movieid + pArgsIn->personid); 
   	if ( IO_SUCCESS == mClientSock->Receive() )
    {
        pArgsIn->result = USER_SUCCESS;
		WriteLcd(Lcd::MOVIE_OUT_IO);
    }
    else
    {
        pArgsIn->result = USER_FAILURE;
		WriteLcd(Lcd::MOVIE_OUT_ERR_IO);
    }
}

void Controls::WriteLcd(const Lcd::CHECK_IO &rcheck)
{
	mLcd->WriteLcd(rcheck);
}

void* Controls::PerformAction(void* obj)
{
    PerformArgs *args = (PerformArgs*)obj;
    (args->obj)->PerformAction(args);
    return args;
}

void Controls::PerformAction(PerformArgs *pArgsIn)
{
    switch ( pArgsIn->type )
    {
        case MEDIA_MOVIE_FIND:
        {
			WriteLcd(Lcd::MOVIE_CHECK_IO);
   	        Read(pArgsIn);
            break;
        }
        case MEDIA_PERSON_FIND:
        {
			WriteLcd(Lcd::PERSON_CHECK_IO);
   	        Read(pArgsIn);
            break;
        }
        case MEDIA_OUT:
        {
            PerformCheckOut(pArgsIn);
            break;
        }
        default:
        {

            break;
        }
    }
}

void* Controls::CheckUpButton(void* obj)
{
    ((Controls *) obj)->CheckUpButton();
    return obj;
}

void Controls::CheckUpButton()
{
    while (1)
    {
        if ( bcm2835_gpio_eds(PIN_UP_BUTTON) )
        {
		    bcm2835_gpio_set_eds(PIN_UP_BUTTON);
            bcm2835_gpio_write(PIN_PURPLE_LIGHT, HIGH);
			mLcd->ArrowUp();
            bcm2835_gpio_write(PIN_PURPLE_LIGHT, LOW);
        }
    }
};

void* Controls::CheckDownButton(void* obj)
{
    ((Controls *) obj)->CheckDownButton();
    return obj;
}

void Controls::CheckDownButton()
{
    while (1)
    {
        if ( bcm2835_gpio_eds(PIN_DOWN_BUTTON) )
        {
		    bcm2835_gpio_set_eds(PIN_DOWN_BUTTON);
            bcm2835_gpio_write(PIN_PURPLE_LIGHT, HIGH);
			mLcd->ArrowDown();
            bcm2835_gpio_write(PIN_PURPLE_LIGHT, LOW);
        }
    }
};

void* Controls::GreenLedBlink(void* obj)
{
    ((Controls *) obj)->GreenLedBlink();
    return obj;
}

void Controls::GreenLedBlink()
{
    while (1)
    {
        delay(500);
        bcm2835_gpio_write(PIN_GREEN_LIGHT, HIGH);
        delay(500);
        bcm2835_gpio_write(PIN_GREEN_LIGHT, LOW);
    }
};

void Controls::SendToLcd(const std::string &rInStr)
{
 	mLcd->ClearDisplay();
    mLcd->SendData((char*)rInStr.c_str(),0x0);
    mLcd->SendCancel(0x1,false);
}

void Controls::SendErrorToLcd(const std::string &rInStr)
{
 	mLcd->ClearDisplay();
    mLcd->SendDataNoArrow((char*)rInStr.c_str());
    mLcd->SendCancel(0x1);
}

void Controls::waitForEncoderTick(uint8_t pin)
{
    bcm2835_gpio_set_eds(pin);//clear the previous event. makes sure that only next event is detected
    while(!bcm2835_gpio_eds(pin)){std::cout<<"waiting for next tick"<<std::endl;};//waits until next event/tick
}

