#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX);

Thread RTtask;
Thread DisplayTask;

volatile int ms      = 0;  // 0 to 999
volatile int seconds = 0;  // 0 to 59
volatile int minutes = 0;  // 0 to 59

void RTtask_thread()
{
    while (1) {
        ThisThread::sleep_for(1);  // 1ms period

        ms++;
        if (ms >= 1000) {
            ms = 0;
            seconds++;
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) {
                    minutes = 0;
                }
            }
        }
    }
}

void Display_thread()
{
    while (1) {
        // Snapshot all variables together to reduce race condition window
        int snap_ms  = ms;
        int snap_sec = seconds;
        int snap_min = minutes;
        int tenths   = snap_ms / 100;

        pc.printf("min:sec  %02d:%02d.%d\r", snap_min, snap_sec, tenths);
        ThisThread::sleep_for(100);  // update every 0.1s
    }
}

int main()
{
    pc.baud(9600);
    pc.printf("\033[2J\033[H");
    pc.printf("RT Clock Started\n\n");

    RTtask.start(RTtask_thread);
    RTtask.set_priority(osPriorityRealtime);

    DisplayTask.start(Display_thread);
    DisplayTask.set_priority(osPriorityHigh);

    while (true) {
        ThisThread::sleep_for(osWaitForever);
    }
}