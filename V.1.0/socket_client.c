#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_LEN 32

int open_tcp(char *server_add,int port)
{
	int sock, len;
	struct sockaddr_in serv;
	socklen_t addlen = sizeof(serv);
	char buf[BUF_LEN];

	sock = socket(PF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(server_add,&serv.sin_addr);

	connect(sock, (struct sockaddr *) &serv, addlen);
	return sock;
}

int close_tcp(int sock)
{
	shutdown (sock, 0);
}
