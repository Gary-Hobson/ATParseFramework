/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-09 10:27:31
 */ 

#include "atparse.h"

int exec_res;

at_status_t at_handle_cplt(const char * str,void * param);
at_status_t _get_rssi(const char * str,void * param);
at_status_t _get_imei(const char * str,void *param);

const at_parser_t at_cmd_list[]=
{
    {{"AT",NULL,BASECMD},       {CRLF,     0}, at_handle_cplt,&exec_res},
    {{"AT+CSQ",NULL,EXEXCMD},   {"+CSQ: ", 1}, _get_rssi,&exec_res},
    {{"AT+CGSN",NULL,EXEXCMD},  {CRLF,     -1},_get_imei,&exec_res},
    {{"AT+CFUN","1",WRITECMD},  {CRLF,     0}, at_handle_cplt,&exec_res},
    {{"AT+CPIN",NULL,READCMD},  {"+CPIN: ",-1},at_handle_cplt,&exec_res},
    {{"AT+CREG",NULL,READCMD},  {"+CREG: ",2}, at_handle_cplt,NULL}
};


at_status_t _get_rssi(const char * str,void * param)
{
    int csq;
    if(str2int(str,&csq)<0)
        return AT_ERROR;  
    printf("RSSI %d\n",csq);
    *(uint16_t *)param=csq;
    if(csq<10)
    {
        printf("信号强度过低\n");
    }
    return AT_SUCCESS;
}

at_status_t _get_imei(const char * str,void *param)
{
    uint16_t i;
    char * imei=(char *)str;
        // param=str;

    while(*str++!='\n');
    *(char *)str='\0';
    printf("IMEI :%s\n",imei);
    return AT_SUCCESS;
}

at_status_t at_errer_handle(at_cmd_t * cmd,const char * param)
{

}

/*AT指令处理完成回调,str参数为空,param为执行结果,0执行失败,1执行成功*/
__WEAK at_status_t at_handle_cplt(const char * str,void * param)
{
    printf("AT CMD PARSE CPL\n");
    return AT_SUCCESS;
}


