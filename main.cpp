#include "mbed.h"
#include <chrono>

#define BUFF_LENGTH 64

int length;
char buffer[BUFF_LENGTH] = {0};
BufferedSerial pc(USBTX, USBRX, 9600);
char buf[32] = {0};
char MSG[32];
int length;

if (uint32_t num = pc.read(buf, sizeof(buf))) {
// String compare buf with the character 1. Equals 0 if match.
if(strcmp(buf, "1") == 0)
{
led = !led;
length = snprintf(MSG,MSG_BUFF_LENGTH, "Received Command LED1\r\n");
pc.write(MSG,length);
}
}

// main() runs in its own thread in the OS
int main()
{

   while (1)  
   {
       length = snprintf(buffer, BUFF_LENGTH, "\r\n{Status:{A:2}}");
       pc.write(buffer, length);
       ThisThread::sleep_for(chrono::seconds(1));
       

   }
}