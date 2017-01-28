/*
<<<<<<< HEAD
 ============================================================================
 Name        : socketlayer.h
 Author      : Dirk Gottschalk
 Version     : 2.0.0
 Copyright   : (c) 2015 Dirk Gottschalk <dirk.gottschalk1980@googlemail.com>
	 Description : Header for socket abstraction layer
	 ============================================================================
	 */
=======
 ***************************************************************************
 *  File:       socketlayer.h
 *  Created:    28.01.2017 16:28:48
 *  Author:     Dirk Gottschalk <user@domain.org>
 *  Copyright:  Copyright 2017 by Dirk Gottschalk
 ***************************************************************************
 */
>>>>>>> b6626bdb1f1e2935978b913a114613908485372b

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

/* Function prototypes */
extern void SLLIB_errorExit (char *error_message); /* Exit with error message */
extern int SLLIB_createSocket (int af, int type, int protocol); /* Create socket */
extern void SLLIB_bindSocket (socket_t *sock, unsigned long adress,
							  unsigned short port); /* Bind socket */
extern void SLLIB_listenSocket ( socket_t *sock); /* Listen on socket */
extern void SLLIB_acceptSocket ( socket_t *new_socket, socket_t *socket); /* Accept conn. */
extern void SLLIB_connectSocket (socket_t *sock, char *serv_addr, unsigned short port); /* Connect socket */
extern void SLLIB_tcpSend ( socket_t *sock, char *data, size_t size); /* Send TCP */
extern int SLLIB_tcpRecv ( socket_t *sock, char *data, size_t size); /* Receive TCP */
extern void SLLIB_udpSend ( socket_t *sock, char *data, size_t size, char *addr,
						   unsigned short port); /* Send UDP */
extern int SLLIB_udpRecv ( socket_t *sock, void *data, size_t size); /* Receive UDP */
extern void SLLIB_closeSocket ( socket_t *sock); /* Close socket */
extern void SLLIB_cleanUp (void); /* Do cleanup */

#endif /* SOCKETLAYER_H_ */
