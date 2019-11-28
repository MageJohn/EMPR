#include "serial.h"

int main(void) {
    serial_init();
    serial_write_b("Hello World!\n\r",16);
    
    return 0;
}
