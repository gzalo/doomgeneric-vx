#include <xc.h>
#include <stdint.h>
#include "fuses.h"

#define _XTAL_FREQ 4000000

#define TRISIO_VAL 0b11111110
#define PIN_TX   GP0
#define PIN_BTN0   GP1
#define PIN_BTN1   GP2
#define PIN_BTN2   GP3
#define PIN_BTN3   GP4
#define PIN_BTN4   GP5

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

    while(PIN_BTN0 && PIN_BTN1 && PIN_BTN2 && PIN_BTN3 && PIN_BTN4);
    
    while(1){
        uint8_t statusByte = 0;
        if(!PIN_BTN4) statusByte |= 1;
        if(!PIN_BTN3) statusByte |= 2;
        if(!PIN_BTN2) statusByte |= 4;
        if(!PIN_BTN1) statusByte |= 8;
        if(!PIN_BTN0) statusByte |= 16;
        sendByte(statusByte);
        __delay_ms(100);
    }
}
