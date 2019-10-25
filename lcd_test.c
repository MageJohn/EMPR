#include "serial.h"
#include "ioboard_i2c.h"
#include "lpc17xx_i2c.h"

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

#define LCD_ADDR 0x38

void poll_busy_flag(void);
void wait_short(int count);

int main(void) {
    I2C_M_SETUP_Type lcd_write;

    serial_init();

    write_usb_serial_blocking("start\n\r", 7);

    ioboard_i2c_init();

    uint8_t init_data[] = {0x00, 0x34, 0x0c, 0x06, 0x35, 0x04, 0x10, 0x42, 0x9f, 0x34, 0x02};

    lcd_write = (I2C_M_SETUP_Type){
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
        .rx_length = 0,
        .tx_data = init_data,
        .tx_length = LEN(init_data),
        .retransmissions_max = 1
    };

    I2C_MasterTransferData(LPC_I2C1, &lcd_write, I2C_TRANSFER_POLLING);
    write_usb_serial_blocking("1\n\r", 3);

    uint8_t two_bytes[] = {0x00, 0x01}; // clear display
    lcd_write.tx_data = two_bytes;
    lcd_write.tx_length = LEN(two_bytes);

    I2C_MasterTransferData(LPC_I2C1, &lcd_write, I2C_TRANSFER_POLLING);
    write_usb_serial_blocking("2\n\r", 3);

    //poll_busy_flag();
    wait_short(200);

    two_bytes[1] = 0x80;
    I2C_MasterTransferData(LPC_I2C1, &lcd_write, I2C_TRANSFER_POLLING);
    write_usb_serial_blocking("3\n\r", 3);

    two_bytes[0] = 0x40;
    two_bytes[1] = 0x64;
    I2C_MasterTransferData(LPC_I2C1, &lcd_write, I2C_TRANSFER_POLLING);
    write_usb_serial_blocking("4\n\r", 3);

    return 0;
}

void poll_busy_flag(void) {
    uint8_t bf_read = 0x00;
    uint8_t bf_response = 0x00;
    I2C_M_SETUP_Type lcd_poll_busy_flag = {
        .sl_addr7bit = LCD_ADDR,
        .tx_data = &bf_read,
        .tx_length = 1,
        .rx_data = &bf_response,
        .rx_length = 1,
        .retransmissions_max = 1
    };

    while (!(bf_response & 0x80)) {
        I2C_MasterTransferData(LPC_I2C1, &lcd_poll_busy_flag, I2C_TRANSFER_POLLING);
    }
}

void wait_short(int count) {
    for (; count > 0; count--) {}
}
