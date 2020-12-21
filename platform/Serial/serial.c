#include "serial.h"
#include <errno.h> 
#include <termios.h>

#define FALSE  -1
#define TRUE   0

/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int speed_arr[] = {B921600, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
          B38400, B19200, B9600, B4800, B2400, B1200, B300, };

int name_arr[] = {921600, 115200, 38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,  
                    19200,  9600, 4800, 2400, 1200,  300, };

void serial_baud(int fd, int speed){
  int   i; 
  int   status; 
  struct termios   Opt;
  tcgetattr(fd, &Opt); 
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) { 
    if  (speed == name_arr[i]) {     

      tcflush(fd, TCIOFLUSH);   

      cfsetispeed(&Opt, speed_arr[i]);  
      cfsetospeed(&Opt, speed_arr[i]);   
      status = tcsetattr(fd, TCSANOW, &Opt);  

      if  (status != 0) {        
        TRACE_ERROR("tcsetattr fd\n");  
        return;    
      }    
      tcflush(fd,TCIOFLUSH);   
      break;
    }  
  }
}

/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
uint32_t serial_config(int fd,int databits,int stopbits,int parity)
{ 
    struct termios options; 
    if  ( tcgetattr( fd,&options)  !=  0) { 
        TRACE_ERROR("SetupSerial 1\n");     
        return(FALSE);  
    }
    options.c_cflag &= ~CSIZE; 
    switch (databits) /*设置数据位数*/
    {   
    case 7:     
        options.c_cflag |= CS7; 
        break;
    case 8:     
        options.c_cflag |= CS8;
        break;   
    default:    
        TRACE_ERROR("Unsupported data size\n"); 
        return (FALSE);  
    }

    /*设置校验位*/
    switch (parity) 
    {   
        case 'n':
        case 'N':    
            options.c_cflag &= ~PARENB;   /* Clear parity enable */
            options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
            break;  
        case 'o':   
        case 'O':     
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
            options.c_iflag |= INPCK;             /* Disnable parity checking */ 
            break;  
        case 'e':  
        case 'E':   
            options.c_cflag |= PARENB;     /* Enable parity */    
            options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
            options.c_iflag |= INPCK;       /* Disnable parity checking */
            break;
        case 'S': 
        case 's':  /*as no parity*/   
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;break;  
        default:   
            TRACE_ERROR("Unsupported parity\n");    
            return (FALSE);  
        }  

    /* 设置停止位*/  
    switch (stopbits)
    {   
        case 1:    
            options.c_cflag &= ~CSTOPB;  
            break;  
        case 2:    
            options.c_cflag |= CSTOPB;  
        break;
        default:    
            TRACE_ERROR("Unsupported stop bits\n");  
            return (FALSE); 
    } 

    /* Set input parity option */ 
    if (parity != 'n')   
        options.c_iflag |= INPCK; 
    tcflush(fd,TCIFLUSH);
    options.c_cc[VTIME] =10; /* 设置超时1 seconds*/   
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    /* struct termio newtio; */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
    options.c_oflag &= ~OPOST; /*Output*/
    if (tcsetattr(fd,TCSANOW,&options) != 0)   
    { 
        TRACE_ERROR("SetupSerial 3 \n");   
        return (FALSE);  
    } 
    return (TRUE);  
}

uint32_t serial_open(char *Dev)
{
    int fd = open( Dev, O_RDWR| O_NOCTTY | O_NDELAY  );         //| O_NOCTTY | O_NDELAY   

    // fcntl(fd,F_SETFL,FNDELAY);
    if (-1 == fd)   
    {           
        strerror(errno);
        TRACE_ERROR("Can't Open Serial Port error:%d \n",errno);
        if (errno == EEXIST )
        {
            TRACE("EEXIST 参数pathname 所指的文件已存在, 却使用了O_CREAT 和O_EXCL 旗标. \r\n");
            
        }else if(errno == EACCES)
        {
            TRACE("EACCESS 参数pathname 所指的文件不符合所要求测试的权限.\r\n");
                
        }else if(errno == EROFS)
        {
            TRACE("EROFS 欲测试写入权限的文件存在于只读文件系统内.\r\n");
        }
        return -1;      
    }

    return fd;
}

int serial_read(int fd, void *data, uint32_t expect_size, uint32_t timeout)
{
    int32_t read_num=0,num=0;
    memset(data,0,expect_size);
    uint32_t start_time=millis();
   
    if(timeout==0)
    {
        num = read(fd, data, expect_size);
        read_num=(num==-1)?0:num;
       // read_num=(errno==0)?-1:read_num;
        return read_num;
    }
    
    while(read_num<expect_size){
       
        num = read(fd, data+read_num, expect_size);
        if(num<0){
            num= 0;
        }
        read_num +=num;
        if(millis()>start_time+timeout){
            break;
        }
    }
    return read_num;
} 


uint32_t serial_write(int fd,void *data, uint32_t size, uint32_t timeout)
{   
    return write(fd, data, size);
}
