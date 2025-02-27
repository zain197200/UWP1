#include "mbed.h"
#include <chrono>
#include <stdio.h>
#include <stdlib.h> // For generating random numbers

#define BUFF_LENGTH 64
#define MSG_BUFF_LENGTH 64
#define BLINKING_RATE 1000ms // Blinking rate of 1 second

int length;
char buffer[BUFF_LENGTH] = {0};
UnbufferedSerial pc(USBTX, USBRX, 115200); // Serial communication over USB
char buf[32] = {0};
char MSG[64];
DigitalOut led(LED1); // LED control pin

// Function to generate random data
int generate_random_data() {
    return rand() % 100; // Random number between 0 and 99
}

int main() {
    length = snprintf(MSG, MSG_BUFF_LENGTH, "{\r\n\"Program Starting\"}}\r\n");
    pc.write(MSG, length);

    // LED stays ON initially
    led = 1; // Turn LED ON initially
    
    while (true) {
        // Continuously send random data every loop iteration
        int random_data = generate_random_data();
        
        // Prepare and send the random data in JSON format
        length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"random_data\":%d}}\r\n", random_data);
        pc.write(MSG, length);

        // Check if there's incoming data (i.e., user presses a key)
        if (uint32_t num = pc.read(buf, sizeof(buf))) {
            if (strcmp(buf, "1") == 0) {
                // Toggle LED when '1' is pressed
                led = !led;  // Toggle the LED
                length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"command\":\"LED1\", \"status\":\"toggled\"}}\r\n");
                pc.write(MSG, length);
            } else if (strcmp(buf, "s") == 0) {
                // Send LED status when 's' is pressed
                length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"status\":{\"LED1\":%d}}\r\n", led.read());
                pc.write(MSG, length);
            }
        }

        // Wait for the next blinking rate cycle before continuing the loop
        ThisThread::sleep_for(BLINKING_RATE); // Delay to ensure we wait 1 second before next random data
    }
}