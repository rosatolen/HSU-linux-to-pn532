#include <stdint.h>
#include <unistd.h>

/*
 * HSU interface on the PN532
 *
 * Data Bit     8 bits
 * Parity Bit   none
 * Stop Bit     1 bit
 * Baud rate    115 200 bauds
 * Data Order   LSB First
 *
 */

// HSU wakeup information on page 99 of User Manual on nxp.com
void wakeup(int ttyFileDescriptor) {
    printf("Waiting for PN532 to wake up...\n");
    unsigned char longPreamble[26] =
        "\x55\x55"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\xFF\x03\xFD\xD4"
        "\x14\x01\x17\x00";
    write(ttyFileDescriptor, longPreamble, sizeof(longPreamble));

    // Expect ACK: 00 00 FF 00 FF 00
}

// Basic HSU communication details on page 40 of User Manual on nxp.com
// Command structure information on page 65 of User Manual on nxp.com
// getFirmwareVerson section on page 73 of User Manual on nxp.com
uint32_t getFirmwareVersion(int ttyFileDescriptor) {
    printf("Getting Firmware Version... \n");
    unsigned char preamble[1] =
        "\x00";  // PRE preamble

    write(ttyFileDescriptor, preamble, 1);

    unsigned char commandStartPart1[1] =
        "\x00";  // START command start part1

    write(ttyFileDescriptor, commandStartPart1, 1);

    unsigned char commandStartPart2[1] =
        "\xFF";  // START command start part2

    write(ttyFileDescriptor, commandStartPart2, 1);

    unsigned char len[1] =
        "\x03";  // LEN length of data in COM and D[] and DCS

    write(ttyFileDescriptor, len, 1);

    unsigned char lcs[1] =
        "\x02";  // LCS length checksum LEN + LCS = 0x00

    write(ttyFileDescriptor, lcs, 1);

    unsigned char dir[1] =
        "\xD4";  // DIR Direction 0xD4 Host to module, 0xD5 module to host

    write(ttyFileDescriptor, dir, 1);

    unsigned char command[1] =
        "\x02";  // COM command, one byte with command code (get firmware version in this case)

    write(ttyFileDescriptor, command, 1);

    unsigned char data[1] =
        "\x00"; // D[] Data Array divided in bytes

    write(ttyFileDescriptor, data, 1);

    unsigned char dcs[1] =
        "\x12";  // DCS Data Checksum (sum of Data + DIR + DCS)

    write(ttyFileDescriptor, dcs, 1);

    unsigned char postamble[8] =
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00";

    write(ttyFileDescriptor, postamble, sizeof(postamble));

    // Wait for pn532 to receive the command
    sleep(0.5);

    unsigned char response[1024];
    int bytesRead=0, loc=0;
    char buf = '\0';

    do {
        bytesRead = read(ttyFileDescriptor, &buf, 1);
        sprintf(&response[loc], "%c", buf);
        loc += bytesRead;
    } while ( buf != '\r' && bytesRead > 0 );

    if (bytesRead < 0)
        perror("TTY ERROR reading\n");
    if (bytesRead == 0)
        perror("Read nothing! :(  \n");
    if (bytesRead > 0)
        printf("Read %d response bytes.\n", bytesRead);

    uint32_t value = 0;

    return value;
}
