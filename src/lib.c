/*
 ============================================================================
 Name        : libslayer.c
 Author      : Dirk Gottschalk
 Version     : 1.1.3
 Copyright   : (c) 2015 Dirk Gottschalk <dirk.gottschalk1980@googlemail.com>
 Description : ContainerStatus Receiver
 ============================================================================
 */

#if __MINGW32__
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windef.h>
#include <winsock2.h>
#include <socketlayer.h>

/* Error message for Syslog  */
void errorExit(char *error_message) {
	fprintf(stderr, "%s: %d\n", error_message, WSAGetLastError());

	exit (EXIT_FAILURE);
}

/* Initializes TCP for Windows ("winsock"),
 * creates a socket
 * returns socket deskriptor . */
int createSocket(int af, int type, int protocol) {
	socket_t sock;
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		errorExit("Winsock could not be initialized!");

	/* Create socket. */
	sock = socket(af, type, protocol);
	if (sock < 0)
		errorExit("Error while creating socket!");
	return sock;
}

/* Binds the socket to address and Port. */
void bindSocket(socket_t *sock, unsigned long adress, unsigned short port) {
	struct sockaddr_in server;

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(adress);
	server.sin_port = htons(port);
	if (bind(*sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
		errorExit("Couldn't bind to socket!");
}

/* Teile dem Socket mit, dass Verbindungswünsche
 * von Clients entgegengenommen werden. */
void listenSocket( socket_t *sock) {
	if (listen(*sock, 5) == -1)
		errorExit("Listen error!");
}

/* Handle connections to the clients.
 * accept() blocks until client accepts connection. */
void acceptSocket( socket_t *socket, socket_t *new_socket) {
	struct sockaddr_in client;
	unsigned int len;

	len = sizeof(client);
	*new_socket = accept(*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == INVALID_SOCKET)
		errorExit("Error on accept()!");
}

/* Baut die Verbindung zum Server auf. */
void connectSocket( socket_t *sock, char *serv_addr, unsigned short port) {
	struct sockaddr_in server;
	struct hostent *host_info;
	unsigned long addr;

	memset(&server, 0, sizeof(server));
	if ((addr = inet_addr(serv_addr)) != INADDR_NONE) {
		/* argv[1] ist eine numerische IP-Adresse. */
		memcpy((char *) &server.sin_addr, &addr, sizeof(addr));
	} else {
		/* Für den Fall der Fälle: Wandle den
		 * Servernamen bspw. "localhost" in eine IP-Adresse um. */
		host_info = gethostbyname(serv_addr);
		if (NULL == host_info)
			errorExit("Unbekannter Server");
		memcpy((char *) &server.sin_addr, host_info->h_addr,
				host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	/* Baue die Verbindung zum Server auf. */
	if (connect(*sock, (struct sockaddr*) &server, sizeof(server)) < 0)
		errorExit("Kann keine Verbindung zum Server herstellen");
}

/* Daten versenden via TCP */
void tcpSend( socket_t *sock, char *data, size_t size) {
	if (send(*sock, data, size, 0) == SOCKET_ERROR)
		errorExit("Fehler bei send()");
}

/* Daten empfangen via TCP */
int tcpRecv( socket_t *sock, char *data, size_t size) {
	int len;
	len = recv(*sock, data, size, 0);
	if (len > 0 || len != SOCKET_ERROR)
		data[len] = '\0';
	else
		errorExit("Fehler bei recv()");
	return len;
}

/* Daten senden via UDP */
void udpSend( socket_t *sock, char *data, size_t size, char *addr,
		unsigned short port) {
	struct sockaddr_in addr_sento;
	struct hostent *h;
	int rc;

	/* IP-Adresse vom Server überprüfen */
	h = gethostbyname(addr);
	if (h == NULL)
		errorExit("Unbekannter Host?");

	addr_sento.sin_family = h->h_addrtype;
	memcpy((char *) &addr_sento.sin_addr.s_addr, h->h_addr_list[0],
			h->h_length);
	addr_sento.sin_port = htons(port);

	rc = sendto(*sock, data, size, 0, (struct sockaddr *) &addr_sento,
			sizeof(addr_sento));
	if (rc == SOCKET_ERROR)
		errorExit("Konnte Daten nicht senden - sendto()");
}

/* Daten empfangen via UDP */
int udpRecv( socket_t *sock, void *data, size_t size) {
	struct sockaddr_in addr_recvfrom;
	unsigned int len;
	int n;

	len = sizeof(addr_recvfrom);
	n = recvfrom(*sock, data, size, 0, (struct sockaddr *) &addr_recvfrom,
			&len);
	if (n == SOCKET_ERROR)
		errorExit("Fehler bei recvfrom()");

	return n;
}

/* Socket schließen und Winsock freigeben */
void closeSocket( socket_t *sock) {
	closesocket(*sock);
}

void cleanUp(void) {
	/* Cleanup Winsock */
	WSACleanup();
	printf("Aufraeuumarbeiten erledigt ...\n");
}
#else
#include "socketlayer.h"

/* Die Funktion gibt aufgetretene Fehler aus und
 * beendet die Anwendung. */
void errorExit(char *error_message) {
	fprintf(stderr, "%s: %s\n", error_message, strerror(errno));
	exit(EXIT_FAILURE);
}

int createSocket(int af, int type, int protocol) {
	socket_t sock;
	const int y = 1;
	/* Erzeuge das Socket. */
	sock = socket(af, type, protocol);
	if (sock < 0)
		errorExit("Fehler beim Anlegen eines Sockets");

	/* Mehr dazu siehe Anmerkung am Ende des Listings ... */
	setsockopt(sock, SOL_SOCKET,
	SO_REUSEADDR, &y, sizeof(int));
	return (sock);
}

/* Erzeugt die Bindung an die Serveradresse,
 * (genauer gesagt an einen bestimmten Port). */
void bindSocket(socket_t *sock, unsigned long adress, unsigned short port) {
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(adress);
	server.sin_port = htons(port);
	if (bind(*sock, (struct sockaddr*) &server, sizeof(server)) < 0)
		errorExit("Kann das Socket nicht \"binden\"");
}

/* Teile dem Socket mit, dass Verbindungswünsche
 * von Clients entgegengenommen werden. */
void listenSocket( socket_t *sock) {
	if (listen(*sock, 5) == -1)
		errorExit("Fehler bei listen");
}

/* Bearbeite die Verbindungswünsche von Clients.
 * Der Aufruf von accept() blockiert so lange,
 * bis ein Client Verbindung aufnimmt. */
void acceptSocket( socket_t *socket, socket_t *new_socket) {
	struct sockaddr_in client;
	unsigned int len;

	len = sizeof(client);
	*new_socket = accept(*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == -1)
		errorExit("Fehler bei accept");
}

/* Baut die Verbindung zum Server auf. */
void connectSocket(socket_t *sock, char *serv_addr, unsigned short port) {
	struct sockaddr_in server;
	struct hostent *host_info;
	unsigned long addr;

	memset(&server, 0, sizeof(server));
	if ((addr = inet_addr(serv_addr)) != INADDR_NONE) {
		/* argv[1] ist eine numerische IP-Adresse */
		memcpy((char *) &server.sin_addr, &addr, sizeof(addr));
	} else {
		/* Für den Fall der Fälle: Wandle den
		 * Servernamen bspw. "localhost" in eine IP-Adresse um. */
		host_info = gethostbyname(serv_addr);
		if (NULL == host_info)
			errorExit("Unbekannter Server");
		memcpy((char *) &server.sin_addr, host_info->h_addr,
		host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	/* Baue die Verbindung zum Server auf. */
	if (connect(*sock, (struct sockaddr *) &server, sizeof(server)) < 0)
		errorExit("Kann keine Verbindung zum Server herstellen");
}

/* Daten versenden via TCP */
void tcpSend( socket_t *sock, char *data, size_t size) {
	if (send(*sock, data, size, 0) == -1)
		errorExit("Fehler bei send()");
}

/* Daten empfangen via TCP */
int tcpRecv( socket_t *sock, char *data, size_t size) {
	unsigned int len;
	len = recv(*sock, data, size, 0);
	if (len > 0 || len != -1) {
		data[len] = '\0';
		return (len);
	} else {
		/* errorExit("Fehler bei recv()"); */
		return (-1);
	}
}
/* Daten senden via UDP */
void udpSend( socket_t *sock, char *data, size_t size, char *addr,
		unsigned short port) {
	struct sockaddr_in addr_sento;
	struct hostent *h;
	int rc;

	/* IP-Adresse des Servers überprüfen */
	h = gethostbyname(addr);
	if (h == NULL)
		errorExit("Unbekannter Host?");

	addr_sento.sin_family = h->h_addrtype;
	memcpy((char *) &addr_sento.sin_addr.s_addr, h->h_addr_list[0],
			h->h_length);
	addr_sento.sin_port = htons(port);

	rc = sendto(*sock, data, size, 0, (struct sockaddr *) &addr_sento,
			sizeof(addr_sento));
	if (rc < 0)
		errorExit("Konnte Daten nicht senden - sendto()");
}

/* Daten empfangen via UDP */
int udpRecv( socket_t *sock, void *data, size_t size) {
	struct sockaddr_in addr_recvfrom;
	unsigned int len;
	int n;

	len = sizeof(addr_recvfrom);
	n = recvfrom(*sock, data, size, 0, (struct sockaddr *) &addr_recvfrom,
			&len);
	if (n < 0) {
		printf("Keine Daten empfangen ...\n");
		return (-1);
	}
	return (n);
}

/* Socket schließen */
void closeSocket( socket_t *sock) {
	close(*sock);
}

/* Unter Linux/UNIX ist nichts zu tun ... */
void cleanUp(void) {
	printf("Aufraeumarbeiten erledigt ...\n");
	return;
}
#endif
