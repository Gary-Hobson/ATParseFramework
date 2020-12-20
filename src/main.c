/*
 * @Description: 
 * @Version: 1.0
 * @Autor: Hobon
 * @Date: 2020-07-06 18:32:51
 */ 

#ifndef TEST

#include "wrappers.h"
#include "air724.h"
#include "string.h"

void print_queue(void);

int main(int argc, char **argv)
{
    char cmd[128]="AT+CSQ\r\n"; 
    char buff[512];
	int fd;
	int data;

	// char *dev  = "/dev/ttyS16"; //´®¿Ú¶þ
    // fd = serial_open(dev);
    
	at_parse_init(SYNC);

	//at_dev_init("/dev/ttyS24");
	
    while (1)
    {   
		static uint32_t current_time=0;
		int nread=0;
		// at_send_cmd(&(at_cmd_list[AT].cmd),at_handle_cplt,&(at_cmd_list[AT].res_info),&nread);
		// delay(5000);
      	// at_send_cmd(&(at_cmd_list[CSQ].cmd),_get_rssi,&(at_cmd_list[CSQ].res_info),&nread);
		// delay(5000);
		// at_send_cmd(&(at_cmd_list[CGSN].cmd),_get_imei,&(at_cmd_list[CGSN].res_info),&nread);
		register_cmd(AT,NULL);
		register_cmd(CSQ,&nread);
		printf("CSQ:%d\n",nread);
		register_cmd(CGSN,NULL);
		// if(millis()>current_time+2000){
		// 	current_time=millis();
		// 	TRACE("Send AT CMD\n");
		// 	at_send_cmd(&(at_cmd_list[AT].cmd),at_handle_cplt,&(at_cmd_list[AT].res_info),&nread);
      	// }

		// if(millis()>current_time+5000){
		// 	current_time=millis();
		// 	TRACE("Send AT+CSQ CMD\n");
		// 	at_send_cmd(&(at_cmd_list[CSQ].cmd),_get_rssi,&(at_cmd_list[CSQ].res_info),&nread);
      	// }
	//	at_resolver();

	//	delay(10000);
    }
}

#endif