/*
 ============================================================================
 Name        : socketlayer.h
 Author      : Dirk Gottschalk
 Version     : 2.0.0
 Copyright   : (c) 2015 Dirk Gottschalk <dirk.gottschalk1980@googlemail.com>
 Description : Header for socket abstraction layer
 ============================================================================
*/

#ifndef SOCKETLAYER_H_
#define SOCKETLAYER_H_

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if __MINGW32__
#include <winsock2.h>
#include <windef.h>

/* Globally defined socket type (WINDOWS) */
#define socket_t SOCKET
#endif

#if __linux__
# include <errno.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>

/* Globally defined socket type (LINUX) */
#define socket_t int
#endif 


/* Funktion prototypes */
extern void errorExit(char *error_message); /* Exit with error message */
extern int createSocket( int af, int type, int protocol ); /* Create socket */
extern void bindSocket(socket_t *sock, unsigned long adress,
                       unsigned short port); /* Bind socket */
extern void listenSocket( socket_t *sock ); /* Listen on socket */
extern void acceptSocket( socket_t *new_socket, socket_t *socket ); /* Accept conn. */
extern void connectSocket(socket_t *sock, char *serv_addr,
                          unsigned short port); /* Connect socket */
extern void tcpSend( socket_t *sock, char *data, size_t size); /* Send TCP */
extern int tcpRecv( socket_t *sock, char *data, size_t size); /* Receive TCP */
extern void udpSend ( socket_t *sock, char *data, size_t size,
                     char *addr, unsigned short port); /* Send UDP */
extern int udpRecv( socket_t *sock, void *data, size_t size); /* Receive UDP */
extern void closeSocket( socket_t *sock ); /* Close socket */
extern void cleanUp(void); /* Do cleanup */

#endif /* SOCKETLAYER_H_ */
