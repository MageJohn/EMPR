#include "ioboard_i2c.h"
#include "lpc17xx_i2c.h"

#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

void init_lcd(void) {
    // assume that i2c is correctly initialised

    I2C_M_SETUP_Type lcd_write;
    uint8_t init_data[] = {0x00, 0x34, 0x0c, 0x06, 0x35, 0x04, 0x10, 0x42, 0x9f, 0x34, 0x02};

    lcd_write = (I2C_M_SETUP_Type){
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
        .rx_length = 0,
        .tx_data = init_data,
        .tx_length = LEN(init_data),
    };

    I2C_MasterTransferData(LPC_I2C1, &lcd_write, I2C_TRANSFER_POLLING);
}

void r_clear_display(void) {
    // clears the display for the R character set
    // assume lcd and i2c initialised

    I2C_M_SETUP_Type clear_packet;
    uint8_t instructions[] = {Control_byte(0, 0), Display_ctl(0, 0, 0), Set_DDRAM(0x00)};
    uint8_t blank_chars[] = {[0] = Control_byte(0, 1), [1 ... 80] = 0x91};

    clear_packet = (I2C_M_SETUP_Type){
        .sl_addr7bit = LCD_ADDR,
        .rx_data = NULL,
        .rx_length = 0,
        .tx_data = instructions,
        .tx_length = LEN(instructions)
    };

    I2C_MasterTransferData(LPC_I2C1, &clear_packet, I2C_TRANSFER_POLLING);

    clear_packet.tx_data = blank_chars;
    clear_packet.tx_length = LEN(blank_chars);

    I2C_MasterTransferData(LPC_I2C1, &clear_packet, I2C_TRANSFER_POLLING);

    instructions[1] = Display_ctl(1, 0, 0);
    clear_packet.tx_data = instructions;
    clear_packet.tx_length = LEN(instructions) - 1;

    I2C_MasterTransferData(LPC_I2C1, &clear_packet, I2C_TRANSFER_POLLING);
}
