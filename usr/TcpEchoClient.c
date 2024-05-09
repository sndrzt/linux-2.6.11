#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/time.h>
#include <sys/wait.h>
#include<errno.h>

int main(int argc,char *argv[])
{
	int iSocketFD;
	char buffer[1000];
	int iIncomingBytes;
	struct sockaddr_in ServerAddress;

	if(argc !=3)
	{
		printf("Usage:./TcpEchoClient.exe \
			<lP address><Port number>in");
		exit( 1 );
	}

	if((iSocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket errorn");
		exit( 1 );
	}

	memset(&ServerAddress,
		0,
		sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	if(connect(iSocketFD,
		(struct sockaddr*)
		&ServerAddress,
		sizeof(ServerAddress)) < 0)
	{
		printf("connect error\n");
		exit(1);
	}

	while(fgets(buffer, 1000, stdin) != NULL)
	{
		if(write(iSocketFD, buffer, strlen(buffer)) < 0)
		{
			perror("send error");
			exit(0);
		}
		if(iIncomingBytes = read(iSocketFD, buffer, sizeof(buffer))> 0)
		{
			printf("%s", buffer);
		}
		memset(buffer, 0, 1000);
	}
	exit(0);
}

