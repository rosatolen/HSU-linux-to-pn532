#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

#define BAUDRATE B115200

int openTty(void) {
    /* Open File Descriptor */
    int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY | O_SYNC );

    /* Error Handling */
    if ( USB < 0 ) {
        perror("TTY ERROR while opening /dev/ttyUSB0");
    }

    /* *** Configure Port *** */
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    /* Set Baud Rate */
    cfsetospeed (&tty, (speed_t) BAUDRATE);
    cfsetispeed (&tty, (speed_t) BAUDRATE);

    /* Setting other Port Configs */
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= 0;                       // set 0 parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    /* Flush Port, then applies attributes */
    //tcflush( USB, TCIFLUSH );

     /* Error Handling */
    if ( tcgetattr ( USB, &tty ) != 0 ) {
        perror("TTY ERROR with tcGETattr");
    }

   /* Set Blocking */
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;            // 1 seconds read timeout

    if (tcsetattr (USB, TCSANOW, &tty) != 0)
        perror ("TTY ERROR setting term attributes");

    return USB;
}
