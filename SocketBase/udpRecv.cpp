#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 9876;
const char IP_DEST[10] = "127.0.0.1";

int main()
{
	char buf[1024];
	struct sockaddr_in addrSrc;
	memset((void*)&addrSrc, 0, sizeof(addrSrc));
	addrSrc.sin_family = AF_INET;
	addrSrc.sin_port = htons(PORT);
	addrSrc.sin_addr.s_addr = inet_addr(IP_DEST);
	socklen_t addrSrc_len = sizeof(addrSrc);

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPv4 UDP
	if(udpSocket < 0)
	{
		printf("Create UDP Socket Error!\n");
		return -1;
	}

	int result = bind(udpSocket, (struct sockaddr*)&addrSrc, sizeof(addrSrc));
	if(result < 0)
	{
		printf("Bind UDP Socket Error!\n");
		return -1;
	}
	printf("Waiting for message:\n");
	int recvTotal = 0;
	int recvNum = 0;
	while(1)
	{
		recvNum = recvfrom(udpSocket, buf, sizeof(buf), 0, (struct sockaddr*)&addrSrc, &addrSrc_len);
		recvTotal += recvNum;
		printf("Received from client:%s\n", buf);
		if(strncmp(buf, "stop", 4) == 0) // End Receive
		{
			printf("Sender told me to end the connection!\n");
			break;
		}
	}

	close(udpSocket);
	return 0;
}
