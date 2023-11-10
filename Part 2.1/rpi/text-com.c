#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int main(int argc, char *argv[]){
   int file, count;

   if(argc!=2){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }

   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   struct termios options;

   tcgetattr(file, &options);

   options.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

   // send the string plus the null character
   if ((count = write(file, argv[1], strlen(argv[1])+1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   unsigned char receive[100];

   if ((count = read(file, (void*)receive, 100))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
       receive[count]=0;  //There is no null character sent by the Arduino

       if (strncmp((char *)receive, "SE", 2) == 0) {
           printf("The following was read from the sensor in [%d]: %s\n", count, receive);
       } else if (strncmp((char *)receive, "MO", 2) == 0) {
           printf("The following was read from the motor in [%d]: %s\n", count, receive);
       } else {
           printf("Source of message unknown!\n");
       }
   }

   close(file);
   return 0;
}
