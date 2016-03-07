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
	struct sockaddr_in addrDest;
	memset((void*)&addrDest, 0, sizeof(addrDest));
	addrDest.sin_family = AF_INET;
	addrDest.sin_port = htons(PORT);
	addrDest.sin_addr.s_addr = inet_addr(IP_DEST);

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPv4 UDP
	if(udpSocket < 0)
	{
		printf("Create UDP Socket Error!\n");
		return -1;
	}
	for(int iter=0; iter<=20; ++iter)
	{
		sprintf(buf, "data packet with ID %d\0", iter);
		sendto(udpSocket, buf, sizeof(buf), 0, (struct sockaddr *)&addrDest, sizeof(addrDest));
	}
	sprintf(buf, "stop\0");
	sendto(udpSocket, buf, sizeof(buf), 0, (struct sockaddr *)&addrDest, sizeof(addrDest));
	
	close(udpSocket);
	printf("Terminating!\n");

	return 0;
}
