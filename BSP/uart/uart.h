#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdbool.h>

extern void uart_init(void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);

#endif


