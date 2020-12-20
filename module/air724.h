/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobson
 * @Date: 2020-07-09 14:55:16
 */ 

#ifndef __AIR724_H_
#define __AIR724_H_

#include "atparse.h"

typedef enum at_cmd_name_t
{
    AT=0,CSQ,CGSN,CFUN,CREG
}at_cmd_name_t;

extern const at_parser_t at_cmd_list[];
at_status_t at_handle_cplt(const char * str,void * param);

at_status_t _get_rssi(const char * str,void * param);

at_status_t _get_imei(const char * str,void *param);
#endif
