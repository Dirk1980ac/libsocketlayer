/*
 ============================================================================
 Name        : socketlayer.c
 Author      : Dirk Gottschalk
 Version     : 1.1.3
 Copyright   : (c) 2015 Dirk Gottschalk <dirk.gottschalk1980@googlemail.com>
 Description : Network Socket abstraction Layer for Windows / Linux
 ============================================================================
 */

#include <socketlayer.h>


/* Source for Linux compile */

/* Returns Errors and exits application. */
void SLLIB_errorExit (char *error_message) {
	fprintf (stderr, "%s: %s\n", error_message, strerror (errno));
	exit (EXIT_FAILURE);
}

/* Create socket */
int SLLIB_createSocket (int af, int type, int protocol) {
#if __MINGW32__
	socket_t sock;
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	SLLIB_errorExit("Winsock could not be initialized!");

	/* Create socket. */
	sock = socket(af, type, protocol);
	if (sock < 0)
	SLLIB_errorExit("Error while creating socket!");
	return sock;
#endif
#if __linux__
	socket_t sock;
	const int y = 1;
	sock = socket (af, type, protocol);
	if (sock < 0)
		SLLIB_errorExit ("Unable to create socket");

	setsockopt (sock, SOL_SOCKET,
	SO_REUSEADDR, &y, sizeof(int));
	return (sock);
#endif
}

/* Binds the socket to address and Port. */
void SLLIB_bindSocket (socket_t *sock, unsigned long adress, unsigned short port) {
#if __MINGW32__
		struct sockaddr_in server;

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(adress);
	server.sin_port = htons(port);
	if (bind(*sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	SLLIB_errorExit("Couldn't bind to socket!");
#endif
#if __linux__
	struct sockaddr_in server;
	memset (&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (adress);
	server.sin_port = htons (port);
	if (bind (*sock, (struct sockaddr*) &server, sizeof(server)) < 0)
		SLLIB_errorExit ("Unable to bind socket");
#endif
}

/* Tells the socket to accept connections. */
void SLLIB_listenSocket ( socket_t *sock) {
	if (listen (*sock, 5) == -1)
		SLLIB_errorExit ("Error: listen");
}

/* Handle connections to the clients.
 * accept() blocks until client accepts connection. */
void SLLIB_acceptSocket ( socket_t *socket, socket_t *new_socket) {
#if __MINGW32__
		struct sockaddr_in client;
	unsigned int len;

	len = sizeof(client);
	*new_socket = accept(*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == INVALID_SOCKET)
	SLLIB_errorExit("Error on accept()!");
#endif
#if __linux__
	struct sockaddr_in client;
	unsigned int len;
	len = sizeof(client);
	*new_socket = accept (*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == -1)
		SLLIB_errorExit ("Error in accept");
#endif
}

/* Connects socket. */
void SLLIB_connectSocket (socket_t *sock, char *serv_addr, unsigned short port) {
	struct sockaddr_in server;
	struct hostent *host_info;
	unsigned long addr;
	memset (&server, 0, sizeof(server));
	if ((addr = inet_addr (serv_addr)) != INADDR_NONE) {
		memcpy ((char *) &server.sin_addr, &addr, sizeof(addr));
	} else {
		/* Resolve hostname */
		host_info = gethostbyname (serv_addr);
		if (NULL == host_info)
			SLLIB_errorExit ("Unknown host");
		memcpy ((char *) &server.sin_addr, host_info->h_addr,
		host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons (port);

	/* Establish connection */
	if (connect (*sock, (struct sockaddr *) &server, sizeof(server)) < 0)
		SLLIB_errorExit ("Could not connect to server");
}

/* Send data via TCP */
void SLLIB_tcpSend ( socket_t *sock, char *data, size_t size) {
#if __MINGW32__
	if (send(*sock, data, size, 0) == SOCKET_ERROR)
		SLLIB_errorExit("ERROR: send()");
#endif
#if __linux__
	if (send (*sock, data, size, 0) == -1)
		SLLIB_errorExit ("ERROR: send()");
#endif
}

/* Receive data via TCP */
int SLLIB_tcpRecv ( socket_t *sock, char *data, size_t size) {
#if __MINGW32__
	int len;
	len = recv(*sock, data, size, 0);
	if (len > 0 || len != SOCKET_ERROR)
		data[len] = '\0';
	else
		SLLIB_errorExit("ERROR: recv()");
	return len;
#endif
#if __linux__
	unsigned int len;
	len = recv (*sock, data, size, 0);
	if (len > 0 || len != -1) {
		data[len] = '\0';
		return (len);
	} else {
		SLLIB_errorExit ("ERROR on recv()");
		return (-1);
	}
#endif
}

/* Send data via UDP */
void SLLIB_udpSend ( socket_t *sock, char *data, size_t size, char *addr, 
	unsigned short port) {
	struct sockaddr_in addr_sento;
	struct hostent *h;
	int rc;

	/* IP-Adresse des Servers überprüfen */
	h = gethostbyname (addr);
	if (h == NULL)
		SLLIB_errorExit ("Unknown host?");

	addr_sento.sin_family = h->h_addrtype;
	memcpy ((char *) &addr_sento.sin_addr.s_addr, h->h_addr_list[0],
			h->h_length);
	addr_sento.sin_port = htons (port);

	rc = sendto (*sock, data, size, 0, (struct sockaddr *) &addr_sento,
			sizeof(addr_sento));
#if __MINGW32__
	if (rc == SOCKET_ERROR)
	SLLIB_errorExit("Could not send data - sendto()");
#endif
#if __linux__
	if (rc < 0)
		SLLIB_errorExit ("Could not send - sendto()");
#endif
}

/* Recieve data via UDP */
int SLLIB_udpRecv ( socket_t *sock, void *data, size_t size) {
	struct sockaddr_in addr_recvfrom;
	unsigned int len;
	int n;

	len = sizeof(addr_recvfrom);
	n = recvfrom (*sock, data, size, 0, (struct sockaddr *) &addr_recvfrom,
			&len);
#if __MINGW32__
		if (n == SOCKET_ERROR)
	SLLIB_errorExit("ERROR: recvfrom()");
#endif
#if __linux__
	if (n < 0) {
		printf ("Nothing received...\n");
		return (-1);
	}
#endif
	return (n);
}

/* Close socket */
void SLLIB_closeSocket ( socket_t *sock) {
#if __MINGW32__
	closesocket(*sock);
#endif
#if __linux__
	close (*sock);
#endif
}

/* Empty under Linux */
void SLLIB_cleanUp (void) {
#if __MINGW32__
	WSACleanup();
#endif
	return;
}

