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

#include <config.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif

#ifdef HAVE_WINDOWS_H
# include <windows.h>
#endif

#ifdef HAVE_WINDEF_H
# include <windef.h>
#endif

/* Globally defined socket type (WINDOWS) */
#if __MINGW32__
# define socket_t SOCKET
#endif

#ifdef HAVE_ERROR_H
# include <errno.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif

#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

/* Globally defined socket type (LINUX) */
#if __linux__
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
