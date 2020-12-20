/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-08 23:23:37
 */ 

#ifndef __ATPARSE_H_
#define __ATPARSE_H_

#include "wrappers.h"

#define CMD_SPLICE(p1,p2) p1 p2

#define TIMEOUT_DEFAULT 1000

#define CR              "\n"
#define LF              "\n"
#define CRLF            CR LF CR LF
/* Delimiter */
#define AT_RECV_PREFIX          "\n\n"
#define AT_SUCCESS_POSTFIX "OK\n\n"
#define AT_FAIL_POSTFIX    "ERROR"
#define AT_SEND_DELIMITER       "\r\n"

#ifndef bool
#define bool unsigned char
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define TRACE_INFO(fmt, ...)  \
				do { \
						printf("%ld,%s,%d::"fmt,HAL_GetTick(), __FILE__,__LINE__,##__VA_ARGS__);\
				} while(0)
				
#define TRACE(fmt, ...)  \
				do { \
						printf(fmt,##__VA_ARGS__);\
				} while(0)				
				
#define TRACE_ERROR(fmt, ...)  \
				do { \
						printf("%s,%d::"fmt, __FILE__,__LINE__,##__VA_ARGS__);\
				} while(0)	


typedef enum at_status_t
{
	AT_ERROR = 0, 
	AT_SUCCESS ,
} at_status_t;

typedef at_status_t (*pFuncCb)(const char* str,void * param);

typedef enum at_mode_t {
    SYNC=0,     //ͬ��
    ASYNC=1       //�첽
}at_mode_t ;

typedef enum at_cmd_type_t
{
    BASECMD,        //AT<x>
	EXEXCMD,        //AT+<X>
	READCMD,        //AT+<X>?
	WRITECMD,       //AT+<X>=<n>
/*    TESECMD       //AT+<X>=?    ��֧�ָ����� */
}at_cmd_type_t;

typedef struct at_cmd_t
{
    char * cmd;
    char * param;
    at_cmd_type_t type;
}at_cmd_t;

typedef struct  at_res_info_t
{
    //������Ӧǰ׺
    const char *        parser_prefix;
    //��Ҫ��ȡ�Ĳ�������
    //����:-1��ȡ���в���,0����ȡ����,n��ȡ��n������
    int                 param_index;   
}at_res_info_t;

typedef struct at_parser_t
{
    at_cmd_t        cmd;            //AT����ṹ��
    at_res_info_t   res_info;     //ATָ������ṹ��
    pFuncCb         callbak;        //�ص�����ָ��
    void *          param;          //�������ַ�ָ��
} at_parser_t;


int str2int(const char *input, void *val);
at_status_t at_parse_init(at_mode_t mode);
void at_send_cmd_str(const char * cmd );
at_status_t at_send_cmd(const at_cmd_t *cmd,pFuncCb callbak,const at_res_info_t * res_info,void * param);


void at_resolver(void);
void register_cmd(int name,void *param);


#endif

