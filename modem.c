/*
 * Copyright 2022, 2023 S. V. Nickolas.
 * Copyright 2023 Marcin Wołoszczuk.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following condition:  The
 * above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#ifdef PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "psp.h"

#define timeval SceNetInetTimeval

#define select  sceNetInetSelect
#define recv    sceNetInetRecv
#define send    sceNetInetSend
#define socket  sceNetInetSocket
#define connect sceNetInetConnect
#define close   sceNetInetClose
#endif

/*
 * Version of modem.c to interface with DJ Sures' emulator.
 * 
 * This is intended as a temporary solution until the emulator is 
 * reimplemented within Marduk itself.
 */

static int status;
static int mosock;

uint8_t modem_bytes_available()
{
 struct timeval timeval;
 fd_set fds;
 int e;
 
 if (!status) return 0;
 
 timeval.tv_sec=0;
 timeval.tv_usec=0;
 
 FD_ZERO(&fds);
 FD_SET(mosock, &fds);
 
 e=select(mosock+1, &fds, 0, 0, &timeval);
 if (e==-1)
 {
  perror("select()");
  return 0;
 }
 
 if (!e) /* Data on 0 sockets */
 {
  return 0;
 }
 return 1;    
}

uint8_t modem_read (uint8_t *b)
{
 if (!status) return 0;
 if (modem_bytes_available()) {
   recv(mosock, b, 1, 0);
   return 1;
 }
 return 0;
}

void modem_write (uint8_t data)
{
 if (status) send(mosock, &data, 1, 0);
 return;
}

int modem_init (char *server, char *port)
{
 int e;
 
 status=0;
 
#ifdef PSP
sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
sceUtilityLoadNetModule(PSP_NET_MODULE_INET);

sceNetInit(64*1024, 32, 2*1024, 32, 2*1024);
sceNetInetInit();
sceNetApctlInit(0x2000, 20);

int net = select_netconfig();
connect_ap(net);

FILE *fp = NULL;
char adapterIP[16];

fp = fopen("marduk.ini", "r");

if (fp == NULL) 
{
 printf("error reading marduk.ini\n");
 fclose(fp);
 return 1;
}

if (fgets(adapterIP, sizeof(adapterIP), fp) == NULL)
{
 printf("File read error.\n");
 fclose(fp);
 return 1;
}

fclose(fp);

printf("Adapter IP: %s\n", adapterIP);

char psp_ip[16];
if (get_ip(psp_ip))
	
printf("PSP's IP: %s\n", psp_ip);
else
{
printf("Could not get PSP IP address\n");
e=-1;
return 0;
}

printf("Connecting...\n");
mosock = sceNetInetSocket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in sa_dst;
memset(&sa_dst, 0, sizeof(struct sockaddr_in));
sa_dst.sin_family = AF_INET;
sa_dst.sin_port = htons(5816);
inet_pton(AF_INET, adapterIP, &sa_dst.sin_addr.s_addr);

e = sceNetInetConnect(mosock, (struct sockaddr *) &sa_dst, sizeof(struct sockaddr_in));

#else
 
 struct addrinfo hints, *result;

 memset(&hints,0,sizeof(struct addrinfo));
 hints.ai_family=AF_INET;
 hints.ai_socktype=SOCK_STREAM;
 hints.ai_flags=(AI_NUMERICHOST | AI_NUMERICSERV);
 hints.ai_protocol=IPPROTO_TCP;
 e=getaddrinfo("127.0.0.1", "5816", &hints, &result);
 
#endif

 if (e)
 {
  #ifdef PSP
  fprintf (stderr, "Modem init failed: %d\n", sceNetInetGetErrno());
  #else
  fprintf (stderr, "Modem init failed: %s\n", gai_strerror(e));
  #endif
  return -1;
 }
 
 #ifndef PSP
 mosock=socket(result->ai_family, result->ai_socktype, result->ai_protocol);
 #endif
 
 if (mosock<0)
 {
  perror ("Could not get a socket");
  #ifdef PSP
  close(mosock);
  #else
  freeaddrinfo(result);
  #endif
  return -1;
 }
 
 #ifndef PSP
 e=connect(mosock, result->ai_addr, result->ai_addrlen);
 freeaddrinfo(result);
 #endif
 
 if (e==-1)
 {
  perror ("Connection to virtual modem failed");
  return -1;
 }
 printf ("Connection to virtual modem succeeded\n");
 status=1;
 
 return 0;
}

void modem_deinit (void)
{
 if (!status) return;
 printf ("Shutting down virtual modem.\n");
 close(mosock);
 //sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
 //sceUtilityUnloadNetModule(PSP_NET_MODULE_INET);
}
