#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX, USBRX);

struct Message {
    int val1;
    int val2;
};

Queue<Message, 10> msgQueue;
MemoryPool<Message, 10> mpool;

Thread nrtThread, rtThread;

// SUB-PART 1: NRT=700, RT=1000  (queue fills up)
// SUB-PART 2: NRT=1000, RT=700  (queue stays empty)
#define NRT_PERIOD  700    // change to 1000 for sub-part 2
#define RT_PERIOD   1000   // change to 700  for sub-part 2

void nrt_producer() {
    while (true) {
        if (msgQueue.full()) {
            printf("Queue is full\n");
        } else {
            Message* msg = mpool.alloc();
            if (msg != NULL) {
                msg->val1 = rand() % 100;
                msg->val2 = rand() % 100;
                msgQueue.put(msg);
                printf("Produced: val1=%d, val2=%d\n",
                       msg->val1, msg->val2);
            }
        }
        ThisThread::sleep_for(NRT_PERIOD);
    }
}

void rt_consumer() {
    while (true) {
        osEvent evt = msgQueue.get(0);
        if (evt.status == osEventMessage) {
            Message* msg = (Message*)evt.value.p;
            printf("Consumed: val1=%d, val2=%d\n",
                   msg->val1, msg->val2);
            mpool.free(msg);
        }
        ThisThread::sleep_for(RT_PERIOD);
    }
}

int main() {
    pc.baud(9600);
    nrtThread.start(nrt_producer);
    rtThread.start(rt_consumer);
    nrtThread.set_priority(osPriorityNormal);
    rtThread.set_priority(osPriorityRealtime);
    while (true) { ThisThread::sleep_for(osWaitForever); }
}