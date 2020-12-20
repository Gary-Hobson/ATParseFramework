/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-09 15:20:07
 */ 
#ifndef __WRAPPWES_H_
#define __WRAPPWES_H_

#include "msp.h"
#include "string.h"
#include "common.h"

int at_dev_init(const char *name );
int at_send_data( uint8_t *data,uint16_t len,uint32_t timeout);
int at_recv_data( uint8_t *data,uint16_t len);


#endif