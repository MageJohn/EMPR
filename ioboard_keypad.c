#include <stdbool.h>

#include "lpc17xx_i2c.h"

#include "ioboard_keypad.h"

// All functions assume that the IO board's I2C interface has been initialised
// with the function ioboard_i2c_init()


uint8_t scancode8b_to_4b(uint8_t scancode8b);
uint8_t onehot4b_to_binary2b(uint8_t onehot4b);


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


bool ioboard_keypad_get_key(uint8_t *pressed) {
    uint8_t tx;
    uint8_t rx;
    I2C_M_SETUP_Type packet = {
        .sl_addr7bit = KEYPAD_ADDR,
        .rx_data = &rx,
        .rx_length = 1,
        .tx_data = &tx,
        .tx_length = 1
    };
    uint8_t scancode8b;
    uint8_t read_key;
    static uint8_t key_repeat[16] = {[0 ... 15] = 0};
    uint8_t row;
    uint8_t scancode;
    bool found_key = false;

    for (tx = ~0x10; tx != 0xff; tx = ~((~tx) << 1)) {
        I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
        scancode8b = ((~rx) & 0x0f) | (~tx);
        read_key = scancode8b_to_4b(scancode8b);

        for (row=0x01; row <= 0x08; row <<= 1) {
            scancode = scancode8b_to_4b(~tx | row);
            if (read_key == scancode) {
                key_repeat[scancode] -= 1;
                if (key_repeat[scancode] == 0xff) {
                    *pressed = scancode;
                    found_key = true;
                }
            } else {
                key_repeat[scancode] = 0;
            }
        }
    }

    return found_key;
}
