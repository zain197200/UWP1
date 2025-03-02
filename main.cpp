#include "mbed.h"
#include <string.h>
#include <stdlib.h>

#define MSG_BUFF_LENGTH 64  // Define buffer size for messages

// Buffers for incoming and outgoing data
char buf[64] = {0};  
char MSG[MSG_BUFF_LENGTH];

// Digital output for controlling LED
DigitalOut led(LED1);

// Serial communication setup using BufferedSerial
BufferedSerial pc(USBTX, USBRX, 115200);

int counter = 0;  // Counter for timing sensor data transmission

// Function to process commands with shorter responses
void process_command(const char* command) {
    if (strcmp(command, "toggleLED") == 0) {  
        // Toggle the LED and send status update
        led = !led;  
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"LED\":\"%s\"}\r\n", led.read() ? "ON" : "OFF");
    } 
    else if (strcmp(command, "LEDStatus") == 0) {  
        // Send current LED status
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"LED\":\"%s\"}\r\n", led.read() ? "ON" : "OFF");
    } 
    else {  
        // Send an error message for invalid commands
        snprintf(MSG, MSG_BUFF_LENGTH, "{\"error\":\"Invalid\"}\r\n");
    }

    // Write the response back to the UWP application
    pc.write(MSG, strlen(MSG));
}

int main() {
    // Send an initial message indicating the MCU is ready
    pc.write("{\"Mbed\":\"Ready\"}\r\n", 17);

    while (true) {
        // Check if data is available from the UWP application
        if (pc.readable()) {
            int num = pc.read(buf, sizeof(buf) - 1);  // Read incoming data
            buf[num] = '\0';  // Ensure null termination

            // Check if received command is "toggleLED" and process it
            if (strstr(buf, "toggleLED")) {
                process_command("toggleLED");
            } 
            // Check if received command is "LEDStatus" and process it
            else if (strstr(buf, "LEDStatus")) {
                process_command("LEDStatus");
            }
        }

        // Send sensor data every 1 second (100 iterations of 10ms)
        if (counter % 100 == 0) {  
            snprintf(MSG, MSG_BUFF_LENGTH, "{\"Sensor1\":%d,\"Sensor2\":%d}\r\n", 
                     rand() % 100, rand() % 100);
            pc.write(MSG, strlen(MSG));  // Send sensor data to UWP application
        }

        counter++;  
        ThisThread::sleep_for(10ms);  // Delay to avoid excessive CPU usage
    }
}