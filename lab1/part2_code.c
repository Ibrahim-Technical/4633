#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX);

Semaphore sem3(1);
Semaphore sem2(0);
Semaphore sem1(0);

Thread threadT1, threadT2, threadT3;

void taskT1() {
    while (true) {
        sem1.acquire();
        ThisThread::sleep_for(2000ms);
        printf("Task T1 executing\n\n");
        sem3.release();
    }
}

void taskT2() {
    while (true) {
        sem2.acquire();
        ThisThread::sleep_for(500ms);
        printf("Task T2 executing\n");
        sem1.release();
    }
}

void taskT3() {
    while (true) {
        sem3.acquire();
        ThisThread::sleep_for(1000ms);
        printf("Task T3 executing\n");
        sem2.release();
    }
}

int main() {
    pc.baud(9600);
    threadT1.start(taskT1);
    threadT2.start(taskT2);
    threadT3.start(taskT3);
    while (true) { ThisThread::sleep_for(osWaitForever); }
}