/*
 * @Author: your name
 * @Date: 2020-12-20 23:04:13
 * @LastEditTime: 2020-12-20 23:04:14
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ATParse/src/wrappers.c
 */

#include "wrappers.h"
#include "serial.h"
#include "common.h"

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <poll.h>
#include <sys/epoll.h>
#include <pthread.h>


int fd ;


void* serial_poll(void * arg);

int at_dev_init(const char * com )
{
    pthread_t serial_thread;
    int  res;

    fd = serial_open(com);
    //serial_baud(fd,115200);
    int iFd;
 
    struct termios opt; 
 
    tcgetattr(iFd, &opt);      
 
    cfsetispeed(&opt, B9600);
    cfsetospeed(&opt, B9600);
 
    
    /*
     * raw mode
     */
    opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG);
    opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
    opt.c_oflag   &=   ~(OPOST);
    opt.c_cflag   &=   ~(CSIZE   |   PARENB);
    opt.c_cflag   |=   CS8;
 
    /*
     * 'DATA_LEN' bytes can be read by serial
     */
    opt.c_cc[VMIN]   =   0;                                      
    opt.c_cc[VTIME]  =   150;
 
    if (tcsetattr(iFd,   TCSANOW,   &opt)<0) {
        return   -1;
    }
    
    if (serial_config(fd,8,1,'N') )  
    {
        TRACE("Set Parity Error\n");
        return -1;
    }

    res = pthread_create(&serial_thread, NULL, serial_poll, fd);

    return res;
}

int at_send_data( uint8_t *data,uint16_t len,uint32_t timeout)
{
    return serial_write( fd, data, len, timeout);
}

/*读取数据,返回值:-1 读取错误,0无可读取数据 ,n 读取到n位数据*/
int at_recv_data( uint8_t *data,uint16_t len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        TRACE("%02X ", data[i]) ;

        if ((i+1)%8 == 0)
            printf("\n");
    }
    printf("\n");
}

void* serial_poll(void * arg)
{
    int	epid;
	struct epoll_event event[1];
    int res, num, len;
    uint8_t buff[256];

    epid = epoll_create(1);
	event[1].events =  EPOLLIN;
	event[1].data.fd = fd;
    res=epoll_ctl(epid, EPOLL_CTL_ADD, fd, &event);
	
    if (res != 0) 
    {
		TRACE_ERROR("set epoll error!\n");
		return -1;
	}

    while(1)
    {
        num = epoll_wait(epid, event, 1, 100);

		if( num == 0)
            continue;
		
		for (int i = 0; i < num; i++) 
		{
			// if ((event[i].events & EPOLLERR)
			// 		|| (event[i].events & EPOLLHUP)
			// 		|| (!(event[i].events & EPOLLIN))) 
			// {
			// 	continue;
            //     TRACE_ERROR("error \n");
			// } 
			// else 
            if (event[i].events & EPOLLIN) 
            {
                size_t count;
                ioctl(fd, FIONREAD, &count);
                len = read(fd, buff, 128);
                at_recv_data(buff, len);
            }
        }   
    }

    return 0;
}