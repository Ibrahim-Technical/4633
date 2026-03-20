#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX); // tx, rx

Thread task1;
Thread task2;
Thread task3;

// Task parameters
#define TASK1_PERIOD 10  // Period of Task 1 in  hundreds of milliseconds
#define TASK2_PERIOD 15   // Period of Task 2 in hundreds of milliseconds
#define TASK3_PERIOD 20   // Period of Task 3 in hundreds of milliseconds

#define TASK1_EXEC_TIME 5 // Execution time of Task 1 in hundreds of milliseconds
#define TASK2_EXEC_TIME 5 // Execution time of Task 2 in hundreds of milliseconds
#define TASK3_EXEC_TIME 5 // Execution time of Task 3 in hundreds of milliseconds

#define TASK1_Priority osPriorityRealtime    //osPriorityIdle, osPriorityLow1
#define TASK2_Priority osPriorityRealtime           //osPriorityIdle, osPriorityLow1
#define TASK3_Priority osPriorityRealtime         //osPriorityIdle, osPriorityLow1

#define WAIT_US 100000

void task1_code()
{
    while (true) {
		//simulate task execution using delay
        int delay=TASK1_EXEC_TIME; 
        while (delay!=0)
        {
        	delay--;
			wait_us(WAIT_US);  //us
			pc.printf("Task 1 %5d\n",delay);
        }
    	ThisThread::sleep_for(TASK1_PERIOD*100); //ms
    }
}

void task2_code()
{
     while (true) {
	//simulate task execution using delay
        int delay=TASK2_EXEC_TIME; 
        while (delay!=0)
        {
            delay--;
			wait_us(WAIT_US);  //us
           	pc.printf("Task 2 %10d\n",delay);
        }
		ThisThread::sleep_for(TASK2_PERIOD*100);
    }
}

void task3_code()
{
    while (true) {
	//simulate task execution using delay
	int delay=TASK3_EXEC_TIME; 
        while (delay!=0)
        {
            delay--;
			wait_us(WAIT_US);  //us
            pc.printf("Task 3 %15d\n",delay);
        }
 		ThisThread::sleep_for(TASK3_PERIOD*100);
    }
}

int main()
{
    pc.baud(9600);
  
    // Start threads
    task1.start(task1_code);
    task2.start(task2_code);
    task3.start(task3_code);

    task1.set_priority(TASK1_Priority);
    task2.set_priority(TASK2_Priority);
    task3.set_priority(TASK3_Priority);

    while (true) {
	ThisThread::sleep_for(osWaitForever);
    }
}