/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-14 00:19:50
 */ 

#ifndef __COMMON_H_
#define __COMMON_H_

#include "msp.h"

int queque_init(void);
int isfull(void);
int isempty(void);
int EnQueue(int data);
int DeQueue(int * data);
int queque_init(void);

#endif 
