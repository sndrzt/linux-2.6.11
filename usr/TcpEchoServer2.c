#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
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
	int iServerActive, iIncomingBytes, iSocketFD, iConnectedFD;
	pid_t pid_tChildPid;
	socklen_t ClientLen;
	struct sockaddr_in ClientAddress;
	struct sockaddr_in ServerAddress;
	char buffer[1000];

	if(argc != 2)
	{
		printf("Usage:\n");
		printf("./TcpEchoServer1 [Port number]\n");
		printf("Port number example: 5000\n");
		exit( 1 );
	}

	if((iSocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket error\n");
		exit( 1 );
	}

	memset(&ServerAddress, 0,
		sizeof(ServerAddress));

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = \
		htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	if(bind(iSocketFD,
		(struct sockaddr*)&ServerAddress,
		sizeof(ServerAddress)) < 0)
	{
		printf("bind error'n");
		exit( 1);
	}
	if(listen(iSocketFD,3 ) < 0)
	{
		printf("listen error\n");
		exit(1);
	}

	iServerActive = 1;
	while(iServerActive)
	{
		ClientLen = sizeof(ClientAddress);
		if((iConnectedFD = accept(iSocketFD, (struct sockaddr *)
			&ClientAddress, &ClientLen)) < 0 )
		{
			printf("accept error");
			exit( 1 );
		}
		if((pid_tChildPid = fork()) == -1)
		{
			printf("accept error");
			exit(1);
		}
		if(pid_tChildPid == 0)
		{
			if(close(iSocketFD == -1))
			{
				printf("close error");
				exit(1);
			}

			printf("Connect from %s'\n",
				inet_ntoa(ClientAddress.sin_addr));

			while( 1)
			{
				memset(buffer, 0, 1000);
				if(iIncomingBytes = 
					read(iConnectedFD, buffer, sizeof(buffer)) > 0)
				{
					printf("%s", buffer);
					if(write(iConnectedFD, buffer, sizeof(buffer)) < 0)
					{
						printf("send error!");
						exit(1);
					}
				}
			}
		}

	}
}

