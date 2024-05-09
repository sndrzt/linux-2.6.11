#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int SockFD, Len;
	struct sockaddr_in Addr;
	int AddrLen = sizeof(struct sockaddr_in);
	char cTemp[1000];

	if(argc != 2)
	{
		printf("Usage : udpserver [port number]\n");
		printf("Example: udpserver 5000\n");
		exit(1);
	}

	if((SockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("socket error!");
		exit(1);
	}

	memset(cTemp, 0, 1000);
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(atoi(argv[1]));
	Addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(SockFD, (const struct sockaddr*)&Addr, sizeof(Addr)) < 0)
	{
		printf("bind error!");
		exit( 1 );
	}

	while(1)
	{
		memset(cTemp, 0, 1000);
		Len = recvfrom(SockFD, cTemp, sizeof(cTemp), 0, (struct sockaddr *)&Addr, &AddrLen);
		if(Len>0)
		{
			printf("receive %s", cTemp);
			printf("Received data = %s, length=%d bytes, received fom %s\n", cTemp,Len, inet_ntoa(Addr.sin_addr));
			sendto(SockFD, cTemp, Len, 0, (struct sockaddr *)&Addr, AddrLen);
		}
	}
}

