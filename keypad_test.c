#include <stdio.h>
#include "serial.h"
#include "ioboard_i2c.h"
#include "lpc17xx_i2c.h"

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
    char out[7];

    serial_init();
    ioboard_i2c_init();

    write_usb_serial_blocking("start\n\r", 7);

    while (1) {
        for (tx = ~0x10; tx != 0xff; tx = ~((~tx) << 1)) {
            I2C_MasterTransferData(LPC_I2C1, &packet, I2C_TRANSFER_POLLING);
            rx = (~rx) & 0x0f;
            if (rx) {
                rx = rx | (~tx);
                snprintf(out, 7, "0x%02x\r\n", rx);
                write_usb_serial_blocking(out, 6);
            }
        }
    }

    return 0;
}
