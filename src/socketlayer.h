/*
 ============================================================================
 Name        : socketlayer.h
 Author      : Dirk Gottschalk
 Version     : 1.1.3
 Copyright   : (c) 2015 Dirk Gottschalk <dirk.gottschalk1980@googlemail.com>
 Description : Network Socket abstraction Layer for Windows / Linux
 ============================================================================
 */

#ifndef SOCKETLAYER_H_
#define SOCKETLAYER_H_

#if __MINGW32__
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <io.h>

/* Globally defined socket type */
#define socket_t SOCKET

#else

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Globally defined socket type */
#define socket_t int

#endif /* __MINGW32__*/

/* Funktion prototypes */
extern void errorExit(char *error_message);
extern int createSocket( int af, int type, int protocol );
extern void bindSocket(socket_t
  *sock, unsigned long adress, unsigned short port);
extern void listenSocket( socket_t *sock );
extern void acceptSocket( socket_t *new_socket, socket_t *socket );
extern void connectSocket(socket_t *sock, char *serv_addr,
 unsigned short port);
extern void tcpSend( socket_t *sock, char *data, size_t size);
extern int tcpRecv( socket_t *sock, char *data, size_t size);
extern void udpSend ( socket_t *sock, char *data, size_t size,
  char *addr, unsigned short port);
extern int udpRecv( socket_t *sock, void *data, size_t size);
extern void closeSocket( socket_t *sock );
extern void cleanUp(void);

#endif /* SOCKETLAYER_H_ */
