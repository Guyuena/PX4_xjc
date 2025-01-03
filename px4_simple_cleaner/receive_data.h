#ifndef RECEIVE_DATA_H
#define RECEIVE_DATA_H

#include <stdint.h>

int uart_init(const char *serial_port);
void start_uart_processing(void);

#endif
