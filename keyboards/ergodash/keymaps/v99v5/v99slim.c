/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "v99slim.h"
#include "wait.h"
#include "debug.h"
#include "print.h"
#include "gpio.h"

#ifndef OPTIC_ROTATED
#define OPTIC_ROTATED false
#endif

// Definitions for the V99 serial line.
#ifndef V99_SCLK_PIN
//#define V99_SCLK_PIN B1
//#define V99_SCLK_PIN D3
//#define V99_SCLK_PIN B7
//#define V99_SCLK_PIN D2
#define V99_SCLK_PIN C6
#endif

#ifndef V99_MOSI_PIN
//#define V99_MOSI_PIN B2
//#define V99_MOSI_PIN D2
//#define V99_MOSI_PIN D5
//#define V99_MOSI_PIN B0
#define V99_MOSI_PIN D1
#endif

#ifndef V99_MISO_PIN
//#define V99_MISO_PIN B3
//#define V99_MISO_PIN F1
//#define V99_MISO_PIN B0
//#define V99_MISO_PIN D5
#define V99_MISO_PIN D2
#endif

#ifndef V99_SS_PIN
//#define V99_SS_PIN B0
#define V99_SS_PIN F1
//#define V99_SS_PIN D2
#endif

// PCB ergldash_ble only breakout
#ifndef V99_MOT_PIN
#define V99_MOT_PIN B6
#endif

#ifdef CONSOLE_ENABLE
void print_byte(uint8_t byte) { dprintf("%c%c%c%c%c%c%c%c|", (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')); }
#endif

// Initialize the V99 serial pins.
void v99_slim_init(void) {

    setPinInput(V99_MOT_PIN);

    setPinOutput(V99_SCLK_PIN);
    setPinOutput(V99_MOSI_PIN);
    setPinOutput(V99_SS_PIN);

    setPinInput(V99_MISO_PIN);

    writePinHigh(V99_SCLK_PIN);
    writePinHigh(V99_MOSI_PIN);
    writePinHigh(V99_SS_PIN);

    writePinHigh(V99_MISO_PIN);

    
}

void v99_wait(void){
    wait_us(2);
}

void v99_slim_serial_out(uint8_t byte) {

    uint8_t bitmask= 0b10000000;

    for(int8_t i = 0; i < 8; i++) {

         //wait_us(1);
         v99_wait();
         writePinLow(V99_SCLK_PIN);

         if(byte & bitmask)
             writePinHigh(V99_MOSI_PIN);
         else
             writePinLow(V99_MOSI_PIN);

         //wait_us(1);
         v99_wait();
         writePinHigh(V99_SCLK_PIN);

         bitmask = bitmask >> 1;
    }


}

uint8_t v99_slim_serial_in(void)
{
    uint8_t byte = 0;

    for(int8_t i = 0; i < 8; i++) {

        //wait_us(1);
        v99_wait();
        writePinLow(V99_SCLK_PIN);

        //byte = (byte << 1) | readPin(V99_MISO_PIN);

        //wait_us(1);
        v99_wait();
        writePinHigh(V99_SCLK_PIN);
        byte = (byte << 1) | readPin(V99_MISO_PIN);
    }

    return byte;
}

void v99_slim_write(uint8_t addr,uint8_t data) {

    addr = 0b10000000 | addr; // MSB = 1;

    writePinLow(V99_SS_PIN);
    //wait_us(1);
    v99_wait();
    v99_slim_serial_out(addr);

    wait_us(10);

    v99_slim_serial_out(data);

    //wait_us(1);
    v99_wait();
    writePinHigh(V99_MOSI_PIN);
    writePinHigh(V99_SS_PIN);
}

uint8_t v99_slim_read(uint8_t addr) {

    uint8_t data = 0;

    addr = 0b01111111 & addr; //addr MSB = 0;

    writePinLow(V99_SS_PIN);
    //wait_us(1);
    v99_wait();

    v99_slim_serial_out(addr);

    //wait_us(1);
    //v99_wait();
    wait_us(10);
    writePinHigh(V99_MOSI_PIN);

    data = v99_slim_serial_in();

    //wait_us(1);
    v99_wait();
    writePinHigh(V99_SS_PIN);

    return data;
}

report_v99_t v99_slim_read_burst(void) {

    uint8_t x = 0;
    uint8_t y = 0;

    report_v99_t data;
    data.dx = 0;
    data.dy = 0;

    if(v99_slim_read(REG_PRODUCT_ID) == V99_ID )
    {
        wait_us(30);
        if(v99_slim_read(REG_MOTION) & FLAG_MOTION) {
            wait_us(30);
            x = v99_slim_read(REG_DELTA_X);
            wait_us(30);
            y = v99_slim_read(REG_DELTA_Y);
        }
    }

    //data.dx = convert_twoscomp(x);
    //data.dy = convert_twoscomp(y);

    data.dx = x;
    data.dy = y;

    return data;
}

// Convert a two's complement byte from an unsigned data type into a signed
// data type.
int8_t convert_twoscomp(uint8_t data) {
    if ((data & 0x80) == 0x80)
        return -128 + (data & 0x7F);
    else
        return data;
}

//void v99_set_cpi(uint8_t cpi) {
//    v99_write_reg(REG_MOUSE_CONTROL2, cpi);
//}

bool v99_check_signature(void) {
/*    uint8_t pid = v99_read_reg(REG_PRODUCT_ID);
    uint8_t rid = v99_read_reg(REG_REVISION_ID);
    uint8_t pid2 = v99_read_reg(REG_PRODUCT_ID2);

    return (pid == 0x12 && rid == 0x01 && pid2 == 0x26);*/
    return true;
}
