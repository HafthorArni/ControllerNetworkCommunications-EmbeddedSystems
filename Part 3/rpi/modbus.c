#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len);
void make_msg(uint8_t ID, uint8_t func, uint16_t reg, uint16_t data, uint8_t* msg);
#define MSG_LEN 8
uint8_t msg[MSG_LEN];

int main(int argc, char *argv[]){
   int file, count;
   uint16_t sens_val;
   uint16_t rpm;
   if(argc!=5){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }

   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   struct termios options;

   tcgetattr(file, &options);

   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   //options.c_lflag &= ~(ICANON | ISIG);
   //options.c_oflag &= ~OPOST;
   tcflush(file, TCIFLUSH);
   cfmakeraw(&options);
   tcsetattr(file, TCSANOW, &options);

   make_msg(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), msg);

   if ((count = write(file, msg, MSG_LEN))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

    printf("Sent request:\t");
    for (int i = 0; i < MSG_LEN; i++) {
        if (msg[i] < 16) printf("0");
        printf("%x ",msg[i]);
    }
    printf("\n");

   usleep(1000000);

   unsigned char receive[20];

   if ((count = read(file, (void*)receive, 20))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
        printf("Received request:\t");
        for (int i = 0; i < MSG_LEN; i++) {
            if (receive[i] < 16) printf("0");
            printf("%x ",receive[i]);
        }
        printf("\n");

        if (receive[0] == 0x01){

        }
        if (receive[0] == 0x02){
            sens_val = (receive[4]<<8)|(receive[5]);
            rpm = sens_val*120/1023;
            make_msg(1, 6, 0, rpm, msg);
            if ((count = write(file, msg, MSG_LEN))<0){
                perror("Failed to write to the motor\n");
                return -1;
            }
            printf("Sent request:\t");
            for (int i = 0; i < MSG_LEN; i++) {
                if (msg[i] < 16) printf("0");
                printf("%x ",msg[i]);
            }
            printf("\n");
      }
   }

    usleep(1000000);

   if ((count = read(file, (void*)receive, 20))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
        printf("Received request:\t");
        for (int i = 0; i < MSG_LEN; i++) {
            if (receive[i] < 16) printf("0");
            printf("%x ",receive[i]);
        }
        printf("\n");

        if (receive[0] == 0x01){

        }
        if (receive[0] == 0x02){
            sens_val = (receive[4]<<8)|(receive[5]);
            rpm = sens_val*120/1023;
            make_msg(1, 6, 0, rpm, msg);
            if ((count = write(file, msg, MSG_LEN))<0){
                perror("Failed to write to the motor\n");
                return -1;
            }
            printf("Sent request:\t");
            for (int i = 0; i < MSG_LEN; i++) {
                if (msg[i] < 16) printf("0");
                printf("%x ",msg[i]);
            }
            printf("\n");
      }
   }

   close(file);
   return 0;
}

uint16_t ModRTU_CRC(uint8_t buf[], int len)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) { // Loop over each bit
            if ((crc & 0x0001) != 0) { // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else {// Else LSB is not set
                crc >>= 1; // Just shift right
            }
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

void make_msg(uint8_t ID, uint8_t func, uint16_t reg, uint16_t data, uint8_t* msg){
    msg[0] = ID;
    msg[1] = func;      //func
    msg[2] = reg >> 8;
    msg[3] = reg & 0x00FF;
    msg[4] = data >> 8;
    msg[5] = data & 0x00FF;
    uint16_t crc = ModRTU_CRC(msg, MSG_LEN-2);
    msg[6] = (uint8_t)(crc >> 8);
    msg[7] = (uint8_t)(crc & 0x00FF);
}