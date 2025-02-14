#include "mbed.h"
#include <chrono>

#define BUFF_LENGTH 64

int length;
char buffer[BUFF_LENGTH] = {0};
BufferedSerial pc(USBTX, USBRX, 9600);
// main() runs in its own thread in the OS
int main()
{
   {
       length = snprintf(buffer, BUFF_LENGTH, "\r\n Hello");
       pc.write(buffer, length);
       ThisThread::sleep_for(chrono::seconds(1));
   }
}
