#pragma once

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

void usage(void);
int serialport_init(const char* serialport, int baud);
int serialport_writebyte(int fd, uint8_t b);
int serialport_write(int fd, const char* str);
int serialport_read_until(int fd, char* buf, char until);



//int left,center,right;

int init_arduio_serial()
{
    int fd=0;
    int baudrate = B9600;  // default
    char *device_id="/dev/ttyACM0";
    fd = serialport_init(device_id, baudrate);
    if(fd==-1) cout<<"Error init_arduion_serial()\n";
    else cout<<"---> ARDUINO CONNECTED <----\n";
    return fd;
}


void send_command(uint8_t n,int fd)
{
int rc;
rc = serialport_writebyte(fd, n);
if(rc==-1) cout<<"Error: send_command()\n";
}

int serialport_writebyte( int fd, uint8_t b)
{
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}

int serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n!=len )
        return -1;
    return 0;
}

int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do {
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );

    buf[i] = 0;  // null terminate the string
    return 0;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;

    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;

    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}





/*
void usage(void) {
    printf("Usage: arduino-serial -p <serialport> [OPTIONS]\n"
    "\n"
    "Options:\n"
    "  -h, --help                   Print this help message\n"
    "  -p, --port=serialport        Serial port Arduino is on\n"
    "  -b, --baud=baudrate          Baudrate (bps) of Arduino\n"
    "  -s, --send=data              Send data to Arduino\n"
    "  -r, --receive                Receive data from Arduino & print it out\n"
    "  -n  --num=num                Send a number as a single byte\n"
    "  -d  --delay=millis           Delay for specified milliseconds\n"
    "\n"
    "Note: Order is important. Set '-b' before doing '-p'. \n"
    "      Used to make series of actions:  '-d 2000 -s hello -d 100 -r' \n"
    "      means 'wait 2secs, send 'hello', wait 100msec, get reply'\n"
    "\n");
}
*/

//int main(int argc, char *argv[])
//{
//    int fd = 0;
//    char serialport[256];
//    int baudrate = B9600;  // default
//    char buf[256];
//    int rc,n;
//
//    if (argc==1) {
//        usage();
//        exit(EXIT_SUCCESS);
//    }
//
//    /* parse options */
//    int option_index = 0, opt;
//    static struct option loptions[] = {
//        {"help",       no_argument,       0, 'h'},
//        {"port",       required_argument, 0, 'p'},
//        {"baud",       required_argument, 0, 'b'},
//        {"send",       required_argument, 0, 's'},
//        {"receive",    no_argument,       0, 'r'},
//        {"num",        required_argument, 0, 'n'},
//        {"delay",      required_argument, 0, 'd'}
//    };
//
//    while(1) {
//        opt = getopt_long (argc, argv, "hp:b:s:rn:d:",
//                           loptions, &option_index);
//        if (opt==-1) break;
//        switch (opt) {
//        case '0': break;
//        case 'd':
//            n = strtol(optarg,NULL,10);
//            usleep(n * 1000 ); // sleep milliseconds
//            break;
//        case 'h':
//            usage();
//            break;
//        case 'b':
//            baudrate = strtol(optarg,NULL,10);
//            break;
//        case 'p':
//            strcpy(serialport,optarg);
//            fd = serialport_init(optarg, baudrate);
//            if(fd==-1) return -1;
//            break;
//        case 'n':
//            n = strtol(optarg, NULL, 10); // convert string to number
//            rc = serialport_writebyte(fd, (uint8_t)n);
//            if(rc==-1) return -1;
//            break;
//        case 's':
//            strcpy(buf,optarg);
//            rc = serialport_write(fd, buf);
//            if(rc==-1) return -1;
//            break;
//        case 'r':
//            serialport_read_until(fd, buf, '\n');
//            printf("read: %s\n",buf);
//            break;
//        }
//    }
//
//    exit(EXIT_SUCCESS);
//} // end main
