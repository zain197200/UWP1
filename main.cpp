#include "mbed.h"
#include <string.h>
#include <stdlib.h>

#define MSG_BUFF_LENGTH 64

char buf[64] = {0};  
char MSG[MSG_BUFF_LENGTH];
DigitalOut led(LED1);
BufferedSerial pc(USBTX, USBRX, 115200);
int counter = 0;

// Function to process commands with shorter responses
void process_command(const char* command) {
    if (strcmp(command, "toggleLED") == 0) {
        led = !led;  
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"LED\":\"%s\"}\r\n", led.read() ? "ON" : "OFF");
    } 
    else if (strcmp(command, "LEDStatus") == 0) {
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"LED\":\"%s\"}\r\n", led.read() ? "ON" : "OFF");
    } 
    else {
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"error\":\"Invalid\"}\r\n");
    }
    pc.write(MSG, strlen(MSG));
}

int main() {
    pc.write("{\"Mbed\":\"Ready\"}\r\n", 17);

    while (true) {
        if (pc.readable()) {
            int num = pc.read(buf, sizeof(buf) - 1);
            buf[num] = '\0';

            if (strstr(buf, "toggleLED")) {
                process_command("toggleLED");
            } 
            else if (strstr(buf, "LEDStatus")) {
                process_command("LEDStatus");
            }
        }

        if (counter % 100 == 0) {  
            snprintf(MSG, MSG_BUFF_LENGTH, "{\"Sensor1\":%d,\"Sensor2\":%d}\r\n", 
                     rand() % 100, rand() % 100);
            pc.write(MSG, strlen(MSG));
        }

        counter++;  
        ThisThread::sleep_for(10ms);
    }
}