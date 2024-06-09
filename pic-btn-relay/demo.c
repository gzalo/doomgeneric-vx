#include <xc.h>
#include <stdint.h>
#include "fuses.h"

#define _XTAL_FREQ 4000000

#define TRISIO_VAL 0b11111110
#define PIN_TX   GP0
#define PIN_RIGHT   GP1
#define PIN_DOWN   GP2
#define PIN_FIRE   GP3
#define PIN_LEFT   GP4
#define PIN_UP   GP5

/*
2 GP5 up
3 GP4 left
4 GP3 (ext pullup) fire
5 GP2 down
6 GP1 right
7 GP0 serial output
*/

void boardInit(void){
    TRISIO = TRISIO_VAL;
    ANSEL = 0b00000000;
    ADCON0 = 0b00000000;
    CMCON0 = 0b00000111;
    CMCON1 = 0b00000000;
    nGPPU = 0;
    WPU = TRISIO_VAL;
    PIN_TX = 1;
}

void sendBit(uint8_t val) {
    PIN_TX = val;
    __delay_us(73); // Adjust for 9600 bps
}

void sendByte(uint8_t data) {
    data <<= 1;
    for (int i = 0; i < 8; i++) {
       sendBit(data & 0x01);
       data >>= 1;
    }
    __delay_us(106);
    PIN_TX = 1;
}

void main(void) {
    boardInit();

    while(PIN_RIGHT && PIN_DOWN && PIN_FIRE && PIN_LEFT && PIN_UP);
    
    while(1){
        uint8_t statusByte = 0;
        if(!PIN_UP) statusByte |= 1;
        if(!PIN_LEFT) statusByte |= 2;
        if(!PIN_FIRE) statusByte |= 4;
        if(!PIN_DOWN) statusByte |= 8;
        if(!PIN_RIGHT) statusByte |= 16;
        sendByte(statusByte);
        __delay_ms(100);
    }
}
