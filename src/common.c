/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-10 20:03:26
 */ 

#include "common.h"

int str2int(const char *input, void *val)
{
    int sign = 0;
    int temp = 0;

    if (input == NULL || val == NULL) {
        return -1;
    }

    while(*input == ' ') {  /* only support skipping space */
        input++;
    }

    if (*input == '+') {
        input++;
    }
    else if (*input == '-') {
        input++;
        sign = -1;
    }

    while (*input != 0) {
        if (*input < '0' || *input > '9') {
            break;
        }

        temp = temp * 10 + (*input - '0');
        input++;
    }

    if (sign == -1) {
        temp = -temp;
    }

    *(int *)val = temp;
    return 1;
}

#define BUFFER_SIZE         6 
#define RING_BUFFER_MASK    BUFFER_SIZE-1

uint16_t queue_list[BUFFER_SIZE];
int head=0,tail=0,queue_size;

int queque_init(void)
{
    head=0;
    tail=0;
}

int isfull(void)
{
    return (((tail+1)%BUFFER_SIZE)==head);
}

int isempty(void)
{
    return (head==tail);
}

int EnQueue(int data)
{
    if(isfull()!=0)
    {
        return -1;
    }
    
    queue_list[tail]=data;
    tail = ((tail+1)%BUFFER_SIZE);

    return 1;
}

int DeQueue(int * data)
{
    if(isempty()){
        return -1;
    }

    *data=queue_list[head];
    head =((head+1)%BUFFER_SIZE);
    return 1;
}

int get_queue_size(void)
{
    return ((tail+BUFFER_SIZE- head ) % BUFFER_SIZE);
}

void print_queue(void)
{
    for (size_t i = 0; i < get_queue_size(); i++)
    {
        printf("%d ",queue_list[i]);
    }
    printf("\n");
}


void TRAE_ARRAY(void * data,uint16_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02x ",((uint8_t*)data)[i]);
    }
    
}
