#include <stdio.h>      // standard input / output functions

#include "tty.h"
#include "pn532.h"

//#include <NdefMessage.h>

// NDEF messages
//#define MAX_PKT_HEADER_SIZE  50
//#define MAX_PKT_PAYLOAD_SIZE 100
//uint8_t txNDEFMessage[MAX_PKT_HEADER_SIZE + MAX_PKT_PAYLOAD_SIZE];
//uint8_t *txNDEFMessagePtr;
//uint8_t txLen;

int main() {
    int tty = openTty();

    //txNDEFMessagePtr = &txNDEFMessage[MAX_PKT_HEADER_SIZE];
    //NdefMessage message = NdefMessage();
    //message.addUriRecord("http://elechouse.com");
    //txLen = message.getEncodedSize();
    //if (txLen <= MAX_PKT_PAYLOAD_SIZE) {
    //    message.encode(txNDEFMessagePtr);
    //}
    //else {
    //    printf("Tx Buffer is too small.");
    //    while (1) {
    //    }
    //}

    printf("Initializing Reader\n");
    wakeup(tty);
    getFirmwareVersion(tty);

    close(tty);
}
