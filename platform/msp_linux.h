/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-10 21:23:41
 */ 
#ifndef __MSP_LINUX_H_
#define __MSP_LINUX_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
// #include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define FALSE  -1
#define TRUE   0

#ifndef   __WEAK
  #define __WEAK     __attribute__((weak))
#endif


uint32_t millis(void);
void delay(uint32_t ms);

#endif
