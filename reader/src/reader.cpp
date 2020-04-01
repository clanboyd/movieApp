#include "Controls.h"
#include "Definitions.h"

int main()
{
    Controls *myControls = new Controls();
    if ( false == myControls->Init() )
    {
        delete myControls;
        return 1;
    }
    pthread_t StartThread;
    pthread_t UpButtonThread;
    pthread_t DownButtonThread;
    pthread_create(&StartThread, NULL, Controls::Start, myControls); 
    pthread_create(&UpButtonThread, NULL, Controls::CheckUpButton, myControls); 
    pthread_create(&DownButtonThread, NULL, Controls::CheckDownButton, myControls); 
    pthread_join(StartThread,NULL);
    pthread_join(UpButtonThread,NULL);
    pthread_join(DownButtonThread,NULL);

    myControls->DeInit();
    delete myControls;
    return 0;
};
