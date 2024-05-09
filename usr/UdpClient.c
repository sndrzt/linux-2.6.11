#include<sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char *argv[])
{
	int SockFD,Len;
	struct sockaddr_in Addr;
	int AddrLen = sizeof(struct sockaddr_in);
	char cTemp[1000];

	if(argc != 3)
	{
		printf("Usage:\n");
		printf("udpclient [IP address] [Port number]nin");
		printf("lP address example: 192.168. 128.128\n");
		printf("Port number example: 5000\n");
		exit(1);
	}

	if((SockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket error");
		exit(1 );
	}

	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(atoi(argv[2]));
	Addr.sin_addr.s_addr = inet_addr(argv[1]);

	while(1)
	{
		memset(cTemp, 0, 1000);
		Len = read(STDIN_FILENO, cTemp, sizeof(cTemp));
		sendto(SockFD, cTemp, Len, 0, (const struct sockaddr*)&Addr, AddrLen);
		Len = recvfrom(SockFD,
			cTemp,
			sizeof(cTemp),
			0,
			( struct sockaddr*)&Addr,&AddrLen);
		printf("receive %s\n",cTemp);
	}

	return 0;
}

