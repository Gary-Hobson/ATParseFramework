/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-08 23:23:13
 */ 

#include "atparse.h"
#include "air724.h"

at_parser_t at_parser;
int mode;

static uint8_t inited=0,handed=1;

void at_resolver(void);

void TRAE_ARRAY(void * data,uint16_t len);

at_status_t at_parse_init(at_mode_t mod)
{
    mode=mod;
    if(inited==0){
        inited=1;
        at_dev_init();
        handed=1;
    }else{
        TRACE_ERROR("The AT parser has been initialized!\n");
        return AT_ERROR;
    }

    return AT_SUCCESS;
}

void at_send_cmd_str(const char * cmd )
{
    at_send_data((uint8_t *)cmd,strlen(cmd),TIMEOUT_DEFAULT);
}

at_status_t at_send_cmd(const at_cmd_t *cmd,pFuncCb callbak,const at_res_info_t * res_info,void * param)
{
 //   assert(inited);
    memcpy((void *)&at_parser.cmd,(void *)cmd,sizeof(at_cmd_t));
    memcpy((void *)&at_parser.res_info,(void *)res_info,sizeof(at_res_info_t));
    at_parser.callbak=callbak;
    at_parser.param=param;
    
    if(handed==1){
        handed=0;
    }else{
        return AT_ERROR;
    }
    switch (cmd->type)
    {
    case READCMD:
        at_send_cmd_str(cmd->cmd);
        at_send_cmd_str(CMD_SPLICE("?",AT_SEND_DELIMITER));
        break;
    case WRITECMD:
         at_send_cmd_str(cmd->cmd);
        at_send_cmd_str("=");
        at_send_cmd_str(cmd->param);
        at_send_cmd_str(CMD_SPLICE("?",AT_SEND_DELIMITER));
    break;
    default:        //基本命令和执行命令使用默认格式发送
        at_send_cmd_str(cmd->cmd);
        at_send_cmd_str(AT_SEND_DELIMITER);
        break;
    }
    
    if(mode==0){
        while (handed==0)
        {
            at_resolver();
        }
        
    }

    return AT_SUCCESS;
}

int get_reply(char * data, uint32_t len)
{
    uint16_t timeout=0,res;
    if(mode==0){
        timeout=TIMEOUT_DEFAULT;
    }else{
        timeout=0;
    }
    res=at_recv_data(data,len,timeout);
    return res;
}

void at_resolver(void)
{
    static char recv_buff[256];
    static uint16_t read_num=0;
    uint16_t offset=0,len;
    char * res;
  
    read_num=get_reply(recv_buff+read_num,sizeof(recv_buff));

    if(read_num==0)
    {
        return;

    }
    //  printf("aread_num:\n");
    res=strstr(recv_buff,AT_FAIL_POSTFIX);
    if(res!=NULL){
        at_parser.param=NULL;
        read_num=0;
        TRACE("AT_FAIL_POSTFIX\n");
        return ;
    }
    
    // printf("recv ::%s\nparser_prefix::%s",recv_buff,at_parser.res_info.parser_prefix);
    // printf("\n\nrecv ::\n");
    // TRAE_ARRAY(recv_buff,read_num);
    // printf("\n\nparser_prefix::\n");
    // TRAE_ARRAY(at_parser.res_info.parser_prefix,strlen(at_parser.res_info.parser_prefix));
    // printf("\n\n");
    res=strstr(recv_buff,at_parser.res_info.parser_prefix);
    if(res==NULL){
        at_parser.param=(void *)-1;
        return ;
    }

    offset=res+strlen(at_parser.res_info.parser_prefix)-recv_buff;
    res=strstr(res,AT_SUCCESS_POSTFIX);
    if(res==NULL){
        at_parser.param=(void *)-1;
        handed=1;
        read_num=0;
        return ;
    }
    *res=0;
    len=res-recv_buff-offset;
    res=recv_buff+offset;
    at_parser.param=(void *)0;
    printf("recv::%s,%d\n",res,len);

    if(at_parser.res_info.param_index>0)
    {
        for (size_t i = 0; i < at_parser.res_info.param_index; i++)
        {
            offset=res-recv_buff;
            res=strchr(recv_buff+offset,',');
            *res=0;
        }
    }
    
    read_num=0;
    at_parser.callbak(recv_buff+offset,at_parser.param);
    handed=1;
}


void register_cmd(int name,void *param)
{
    if(param==NULL)
    {
        param=at_cmd_list[name].param;
    }
    at_send_cmd(&(at_cmd_list[name].cmd),at_cmd_list[name].callbak ,&(at_cmd_list[name].res_info),param);
}
