#include <stdio.h>
#include "serial.h"
#include "ioboard_i2c.h"
#include "lpc17xx_i2c.h"
#include "ioboard_lcd.h"

uint8_t scancode8b_to_4b(uint8_t scancode8b);
uint8_t onehot4b_to_binary2b(uint8_t onehot4b);

uint8_t scancode_lut[16] = {
    0xc4, 0xc3, 0xc2, 0xc1,
    0xa3, 0xb9, 0xb6, 0xb3,
    0xb0, 0xb8, 0xb5, 0xb2,
    0xaa, 0xb7, 0xb4, 0xb1
};

int main(void){
    uint8_t tx;
    uint8_t rx;
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = 0x21,
        .rx_data = &rx,
        .rx_length = 1,
        .tx_data = &tx,
        .tx_length = 1
    };
    char out[6];
    uint8_t scancode8b;
    uint8_t read_key;
    uint8_t key_repeat[16] = {[0 ... 15] = 0};

    uint8_t lcd_bytes[] = {Control_byte(0, 0), Set_DDRAM(0x00)};

    serial_init();
    ioboard_i2c_init();
    ioboard_lcd_init();
    ioboard_lcd_clear_display();
    ioboard_lcd_send_bytes(lcd_bytes, 2);

    lcd_bytes[0] = Control_byte(0, 1);

    write_usb_serial_blocking("start\n\r", 7);

    while (1) {
        for (tx = ~0x10; tx != 0xff; tx = ~((~tx) << 1)) {
            I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
            scancode8b = ((~rx) & 0x0f) | (~tx);
            read_key = scancode8b_to_4b(scancode8b);

            uint8_t row;
            uint8_t scancode;
            for (row=0x01; row <= 0x08; row <<= 1) {
                scancode = scancode8b_to_4b(~tx | row);
                if (read_key == scancode) {
                    key_repeat[scancode] -= 1;
                    if (key_repeat[scancode] == 0xff) {
                        snprintf(out, 6, "0x%01x\r\n", scancode);
                        write_usb_serial_blocking(out, 6);
                        lcd_bytes[1] = scancode_lut[scancode];
                        ioboard_lcd_send_bytes(lcd_bytes, 2);
                    }
                } else {
                    key_repeat[scancode] = 0;
                }
            }
        }
    }

    return 0;
}


uint8_t scancode8b_to_4b(uint8_t scancode8b) {
    uint8_t upper_nibble = onehot4b_to_binary2b((scancode8b >> 4));
    uint8_t lower_nibble = onehot4b_to_binary2b(scancode8b);
    if (lower_nibble < 0xff && upper_nibble < 0xff) {
        return (upper_nibble << 2) | lower_nibble;
    } else {
        return 0xff;
    }
}


uint8_t onehot4b_to_binary2b(uint8_t onehot4b) {
    onehot4b = onehot4b & 0x0f;
    switch (onehot4b) {
        case 0x01:
            return 0x00;
        case 0x02:
            return 0x01;
        case 0x04:
            return 0x02;
        case 0x08:
            return 0x03;
        default:
            return 0xff;
    }
}
