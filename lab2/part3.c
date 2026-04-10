#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX);

Thread RTtask;
Thread DisplayTask;
Thread InputTask;

volatile int  ms      = 0;
volatile int  seconds = 0;
volatile int  minutes = 0;
volatile bool running = false;  // starts paused

void RTtask_thread()
{
    while (1) {
        ThisThread::sleep_for(1);
        if (!running) continue;

        ms++;
        if (ms >= 1000) {
            ms = 0;
            seconds++;
            if (seconds >= 60) {
                seconds = 0;
                minutes++;
                if (minutes >= 60) minutes = 0;
            }
        }
    }
}

void Display_thread()
{
    while (1) {
        int snap_ms  = ms;
        int snap_sec = seconds;
        int snap_min = minutes;
        int tenths   = snap_ms / 100;
        const char* status = running ? "RUNNING" : "PAUSED ";

        pc.printf("min:sec  %02d:%02d.%d  [%s]\r", snap_min, snap_sec, tenths, status);
        ThisThread::sleep_for(100);
    }
}

void Input_thread()
{
    while (1) {
        if (pc.readable()) {
            char c = pc.getc();
            if (c == 'S' || c == 's') {
                running = !running;
            }
            if (c == 'R' || c == 'r') {
                running = false;
                ms      = 0;
                seconds = 0;
                minutes = 0;
            }
        }
        ThisThread::sleep_for(50);
    }
}

int main()
{
    pc.baud(9600);
    pc.printf("\033[2J\033[H");
    pc.printf("S = Start/Stop\n");
    pc.printf("R = Reset\n\n");

    RTtask.start(RTtask_thread);
    RTtask.set_priority(osPriorityRealtime);

    DisplayTask.start(Display_thread);
    DisplayTask.set_priority(osPriorityHigh);

    InputTask.start(Input_thread);
    InputTask.set_priority(osPriorityNormal);

    while (true) {
        ThisThread::sleep_for(osWaitForever);
    }
}