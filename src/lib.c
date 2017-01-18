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

#if __MINGW32__
/* Source for MinGW compile */

/* Error message for Syslog  */
void errorExit(char *error_message) {
	fprintf(stderr, "%s: %d\n", error_message, WSAGetLastError());
	exit (EXIT_FAILURE);
}

/* Initializes TCP for Windows ("winsock"),
 * creates a socket
 * returns socket deskriptor . */
int SLLIB_createSocket(int af, int type, int protocol) {
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
void SLLIB_bindSocket(socket_t *sock, unsigned long adress, unsigned short port) {
	struct sockaddr_in server;

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(adress);
	server.sin_port = htons(port);
	if (bind(*sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	errorExit("Couldn't bind to socket!");
}

/* Tells the socket to accept connections. */
void SLLIB_listenSocket( socket_t *sock) {
	if (listen(*sock, 5) == -1)
	errorExit("Listen error!");
}

/* Handle connections to the clients.
 * accept() blocks until client accepts connection. */
void SLLIB_acceptSocket( socket_t *socket, socket_t *new_socket) {
	struct sockaddr_in client;
	unsigned int len;

	len = sizeof(client);
	*new_socket = accept(*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == INVALID_SOCKET)
	errorExit("Error on accept()!");
}

/* Connects socket. */
void SLLIB_connectSocket( socket_t *sock, char *serv_addr, unsigned short port) {
	struct sockaddr_in server;
	struct hostent *host_info;
	unsigned long addr;

	memset(&server, 0, sizeof(server));
	if ((addr = inet_addr(serv_addr)) != INADDR_NONE) {
		memcpy((char *) &server.sin_addr, &addr, sizeof(addr));
	} else {
		/* Resolve hostname */
		host_info = gethostbyname(serv_addr);
		if (NULL == host_info)
		errorExit("Unknown server");
		memcpy((char *) &server.sin_addr, host_info->h_addr,
				host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	/* Establish connection. */
	if (connect(*sock, (struct sockaddr*) &server, sizeof(server)) < 0)
	errorExit("Unable to connect to server");
}

/* Send data via TCP */
void SLLIB_tcpSend( socket_t *sock, char *data, size_t size) {
	if (send(*sock, data, size, 0) == SOCKET_ERROR)
	errorExit("ERROR: send()");
}

/* Receive data via TCP */
int SLLIB_tcpRecv( socket_t *sock, char *data, size_t size) {
	int len;
	len = recv(*sock, data, size, 0);
	if (len > 0 || len != SOCKET_ERROR)
	data[len] = '\0';
	else
	errorExit("ERROR: recv()");
	return len;
}

/* Send data via UDP */
void SLLIB_udpSend( socket_t *sock, char *data, size_t size, char *addr,
		unsigned short port) {
	struct sockaddr_in addr_sento;
	struct hostent *h;
	int rc;

	/* Resolve hostname */
	h = gethostbyname(addr);
	if (h == NULL)
	errorExit("Unknown host?");

	addr_sento.sin_family = h->h_addrtype;
	memcpy((char *) &addr_sento.sin_addr.s_addr, h->h_addr_list[0],
			h->h_length);
	addr_sento.sin_port = htons(port);

	rc = sendto(*sock, data, size, 0, (struct sockaddr *) &addr_sento,
			sizeof(addr_sento));
	if (rc == SOCKET_ERROR)
	errorExit("Could not send data - sendto()");
}

/* Receive data via UDP */
int SLLIB_udpRecv( socket_t *sock, void *data, size_t size) {
	struct sockaddr_in addr_recvfrom;
	unsigned int len;
	int n;

	len = sizeof(addr_recvfrom);
	n = recvfrom(*sock, data, size, 0, (struct sockaddr *) &addr_recvfrom,
			&len);
	if (n == SOCKET_ERROR)
	errorExit("ERROR: recvfrom()");

	return n;
}

/* Close socket and release Winsock */
void SLLIB_closeSocket( socket_t *sock) {
	closesocket(*sock);
}

void SLLIB_cleanUp(void) {
	/* Cleanup Winsock */
	WSACleanup();
	printf("Done cleanup...\n");
}

#endif

#if __linux__
/* Source for Linux compile */

/* Returns Errors and exits application. */
void errorExit (char *error_message) {
	fprintf (stderr, "%s: %s\n", error_message, strerror (errno));
	exit (EXIT_FAILURE);
}

/* Create socket */
int SLLIB_createSocket (int af, int type, int protocol) {
	socket_t sock;
	const int y = 1;
	sock = socket (af, type, protocol);
	if (sock < 0)
		errorExit ("Unable to create socket");

	setsockopt (sock, SOL_SOCKET,
	SO_REUSEADDR, &y, sizeof(int));
	return (sock);
}

/* Binds the socket to address and Port. */
void SLLIB_bindSocket (socket_t *sock, unsigned long adress, unsigned short port) {
	struct sockaddr_in server;
	memset (&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (adress);
	server.sin_port = htons (port);
	if (bind (*sock, (struct sockaddr*) &server, sizeof(server)) < 0)
		errorExit ("Unable to bind socket");
}

/* Tells the socket to accept connections. */
void SLLIB_listenSocket ( socket_t *sock) {
	if (listen (*sock, 5) == -1)
		errorExit ("Error: listen");
}

/* Handle connections to the clients.
 * accept() blocks until client accepts connection. */
void SLLIB_acceptSocket ( socket_t *socket, socket_t *new_socket) {
	struct sockaddr_in client;
	unsigned int len;

	len = sizeof(client);
	*new_socket = accept (*socket, (struct sockaddr *) &client, &len);
	if (*new_socket == -1)
		errorExit ("Error in accept");
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
			errorExit ("Unknown host");
		memcpy ((char *) &server.sin_addr, host_info->h_addr,
		host_info->h_length);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons (port);

	/* Establish connection */
	if (connect (*sock, (struct sockaddr *) &server, sizeof(server)) < 0)
		errorExit ("Could not connect to server");
}

/* Send data via TCP */
void SLLIB_tcpSend ( socket_t *sock, char *data, size_t size) {
	if (send (*sock, data, size, 0) == -1)
		errorExit ("Fehler bei send()");
}

/* Receive data via TCP */
int SLLIB_tcpRecv ( socket_t *sock, char *data, size_t size) {
	unsigned int len;
	len = recv (*sock, data, size, 0);
	if (len > 0 || len != -1) {
		data[len] = '\0';
		return (len);
	} else {
		errorExit ("ERROR on recv()");
		return (-1);
	}
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
		errorExit ("Unknown host?");

	addr_sento.sin_family = h->h_addrtype;
	memcpy ((char *) &addr_sento.sin_addr.s_addr, h->h_addr_list[0],
			h->h_length);
	addr_sento.sin_port = htons (port);

	rc = sendto (*sock, data, size, 0, (struct sockaddr *) &addr_sento,
			sizeof(addr_sento));
	if (rc < 0)
		errorExit ("Could not send - sendto()");
}

/* Recieve data via UDP */
int SLLIB_udpRecv ( socket_t *sock, void *data, size_t size) {
	struct sockaddr_in addr_recvfrom;
	unsigned int len;
	int n;

	len = sizeof(addr_recvfrom);
	n = recvfrom (*sock, data, size, 0, (struct sockaddr *) &addr_recvfrom,
			&len);
	if (n < 0) {
		printf ("Nothing received...\n");
		return (-1);
	}
	return (n);
}

/* Close socket */
void SLLIB_closeSocket ( socket_t *sock) {
	close (*sock);
}

/* Empty under Linux */
void SLLIB_cleanUp (void) {
	return;
}
#endif
