#ifndef PSP_H
#define PSP_H

#include <pspkernel.h>

extern int run;

int select_netconfig();
int connect_ap(int conn_n);
int get_ip(char *ip);


int ExitCallback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupExitCallback();

#endif
