#include "mbed.h"
#include <chrono>
#include <stdio.h>

#define BUFF_LENGTH 64
#define MSG_BUFF_LENGTH 32
#define BLINKING_RATE 10ms
int length;
char buffer[BUFF_LENGTH] = {0};
UnbufferedSerial pc(USBTX, USBRX, 9600);
char buf[32] = {0};
char MSG[32];
DigitalOut led(LED1);
DigitalOut led1(D2);

int main()
{
    length = snprintf(MSG,MSG_BUFF_LENGTH,"\r\nProgramme Starting\r\n");
    pc.write(MSG,length);
    while(true) {
        led = !led;
        if (uint32_t num = pc.read(buf, sizeof(buf))) {
            if(strcmp(buf, "1") == 0)
        {
            led = !led;
            length = snprintf(MSG,MSG_BUFF_LENGTH,"Received Command LED1\r\n");
            pc.write(MSG,length);
        } else if (strcmp(buf, "s") == 0) {
            length = snprintf(MSG,MSG_BUFF_LENGTH,"{status:{LED1:%d}}\r\n",led1.read());
            pc.write(MSG,length);   
        }
        }

        ThisThread::sleep_for(BLINKING_RATE);
    }
}