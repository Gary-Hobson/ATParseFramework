/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobon
 * @Date: 2020-07-06 18:32:51
 */ 

#ifdef TEST

#include "wrappers.h"
#include <pthread.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
    char cmd[128]="AT+CSQ\r\n"; 
	int fd;

    pthread_t serial_thread, at_parse_thread;

	char *dev  = "/dev/ttyS1";
    at_dev_init(dev);

    while (1)
    {   
        at_send_data(cmd,strlen(cmd),1000);
        delay(2000);
    }
}

#endif