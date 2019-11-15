#include <stdio.h>
#include "serial.h"
#include "lpc17xx_i2c.h"
#include "ioboard_i2c.h"
#include "i2c_scanner.h"

#define ADDR_OUT_LEN 7

int i2c_scan(void) {
    I2C_M_SETUP_Type scanning_packet;
    char output[34];
    uint8_t data;
    char formatted_address[ADDR_OUT_LEN];
    int i;
    uint8_t n_devices = 0;

    serial_init();

    ioboard_i2c_init();

    write_usb_serial_blocking("Starting Scan", 15);

    n_devices = 0;
    data = 1;

    scanning_packet = (I2C_M_SETUP_Type){
        .rx_data = NULL,
        .rx_length = 0,
        .tx_data = &data,
        .tx_length = 1,
    };

    for (i = 0; i < 128; i++) {
        if (i % 8 == 0) {
            write_usb_serial_blocking("\n\r", 2);
        }

        scanning_packet.sl_addr7bit = i;
        if (I2C_MasterTransferData(
                LPC_I2C1,
                &scanning_packet,
                I2C_TRANSFER_POLLING) == SUCCESS) {
            snprintf(formatted_address, ADDR_OUT_LEN, " 0x%02x ", i);
            write_usb_serial_blocking(formatted_address, ADDR_OUT_LEN - 1);
            n_devices++;
        } else {
            write_usb_serial_blocking("  --  ", ADDR_OUT_LEN);
        }
    }

    write_usb_serial_blocking("\n\r", 2);

    snprintf(output, 34, "%d devices connected to i2c bus\n\r", n_devices);
    write_usb_serial_blocking(output, 33);

    return 0;
}
