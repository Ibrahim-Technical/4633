#include <stdio.h>

#define SIZE 5

int buffer[SIZE];
int head = 0;   // Read index
int tail = 0;   // Write index
int count = 0;  // Number of items in buffer

// Write (Producer)
int write(int data) {
    if (count == SIZE) {
        printf("Buffer FULL\n");
        return -1;  // overflow
    }

    buffer[tail] = data;
    tail = (tail + 1) % SIZE;
    count++;

    return 0;
}

// Read (Consumer)
int read(int *data) {
    if (count == 0) {
        printf("Buffer EMPTY\n");
        return -1;  // underflow
    }

    *data = buffer[head];
    head = (head + 1) % SIZE;
    count--;

    return 0;
}

int main() {
    int value;

    write(10);
    write(20);
    write(30);

    read(&value);
    printf("Read: %d\n", value);

    write(40);
    write(50);
    write(60);  // This may cause full

    while (read(&value) == 0) {
        printf("Read: %d\n", value);
    }

    return 0;
}