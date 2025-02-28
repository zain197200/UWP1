#include "mbed.h"
#include <string>
#include <stdlib.h>

#define MSG_BUFF_LENGTH 64

int length;
char buf[64] = {0};
char MSG[64];
DigitalOut led(LED1);
BufferedSerial pc(USBTX, USBRX, 115200);

// Function to process incoming JSON command
void process_command(const char* command) {
    if (strcmp(command, "toggleLED") == 0) {
        led = !led;  // Toggle LED state
        length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"command\":\"LED1\", \"status\":\"toggled\"}}\r\n");
        pc.write(MSG, length);
        printf("LED Toggled\n");  // Debugging message
    } else if (strcmp(command, "LEDStatus") == 0) {
        length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"LED1\":%d}}\r\n", led.read());
        pc.write(MSG, length);
        printf("LED Status: %d\n", led.read());  // Debugging message
    } else {
        // Unknown command
        length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"error\":\"Unknown command\"}}\r\n");
        pc.write(MSG, length);
        printf("Unknown command received\n");  // Debugging message
    }
}

int main() {
    while (true) {
        // Check for incoming data
        uint32_t num = pc.read(buf, sizeof(buf) - 1);  // Read incoming data into buffer

        if (num > 0) {
            buf[num] = '\0'; // Null-terminate the string to safely process it
            printf("Received data: %s\n", buf);  // Debugging message to print received data

            // Simple JSON parsing to check for commands
            if (strstr(buf, "{\"command\":\"toggleLED\"}") != NULL) {
                process_command("toggleLED");
            } else if (strstr(buf, "{\"command\":\"LEDStatus\"}") != NULL) {
                process_command("LEDStatus");
            } else {
                length = snprintf(MSG, MSG_BUFF_LENGTH, "{\"error\":\"Invalid JSON format or unknown command\"}\r\n");
                pc.write(MSG, length);
                printf("Invalid JSON or unknown command received\n");  // Debugging message
            }
        }

        // Add a delay to avoid overwhelming the processor with constant reading
        ThisThread::sleep_for(100ms);
    }
}
