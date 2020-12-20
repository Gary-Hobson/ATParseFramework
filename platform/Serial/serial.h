/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobon
 * @Date: 2020-07-06 17:56:43
 */ 
#ifndef __LINUX_SERIAL_H_
#define __LINUX_SERIAL_H_

#include "msp_linux.h"


#define TRACE_INFO(fmt, ...)  \
				do { \
						printf("%ld,%s,%d::"fmt,HAL_GetTick(), __FILE__,__LINE__,##__VA_ARGS__);\
				} while(0)
				
#define TRACE(fmt, ...)  printf(fmt,##__VA_ARGS__)
				
#define TRACE_ERROR(fmt, ...)  \
				do { \
						printf("error> %s,%d::"fmt, __FILE__,__LINE__,##__VA_ARGS__);\
				} while(0)	


void        serial_baud(int fd, int speed);
uint32_t    serial_config(int fd,int databits,int stopbits,int parity);
uint32_t    serial_open(char *Dev);
int    		serial_read(int fd, void *data, uint32_t expect_size, uint32_t timeout);
uint32_t    serial_write(int fd,void *data, uint32_t size, uint32_t timeout);


#endif