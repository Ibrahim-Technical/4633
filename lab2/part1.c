#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX); // tx, rx

Thread RTtask;
Thread Showsec;

int RTseconds;
int NRTseconds;

void RTtask_thread()
{   
    int count=0;
    while(1){
            count+=1;
            if(count==1e3)
            {
                count=0;
                RTseconds+=1;
            }
        ThisThread::sleep_for(1); //in ms
        }
}

void Showsec_thread()
{
    while(1)
    {
        pc.printf("NRT Time: %ds RT Time: %ds\r",NRTseconds, RTseconds);
        ThisThread::sleep_for(500);
    }
}

int main()
{
	pc.baud(9600);
    // ANSI escape code to clear screen and move cursor to home position
    pc.printf("\033[2J\033[H");
    pc.printf("STARTED\n");

    RTtask.start(RTtask_thread);
    RTtask.set_priority(osPriorityRealtime);

    Showsec.start(Showsec_thread);
    Showsec.set_priority(osPriorityHigh1);

    int count=0;
 
     while (true) {
        wait_us(1e3); //in us;   wait_us():count==  1e6:1; 1e3:1e3; 1e2:1e4; 1e1:1e5
        count+=1;
        if(count==1e3)
        {
            count=0;
            NRTseconds+=1;
        }
    }
}
