#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>


const int PORT = 12000;
const char IP_DEST[15] = "222.196.33.159";

int main()
{
	struct timeval start, stop;
	char buf[512];
	memset(buf, 0, sizeof(buf));
	struct sockaddr_in addrDest;
	memset((void*)&addrDest, 0, sizeof(addrDest));
	addrDest.sin_family = AF_INET;
	addrDest.sin_port = htons(PORT);
	addrDest.sin_addr.s_addr = inet_addr("222.196.33.159");

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPv4 UDP
	if(udpSocket < 0)
	{
		printf("Create UDP Socket Error!\n");
		return -1;
	}
	int sendTotal = 0;
	int sendNum = 0;
	int sendCnt = 0;
	gettimeofday(&start, 0);
	for(int iter=0; iter<16000; ++iter)
	{
		//sprintf(buf, "data packet with ID %d\0", iter);
		sendNum = sendto(udpSocket, buf, sizeof(buf), 0, (struct sockaddr *)&addrDest, sizeof(addrDest));
		if(sendNum > 0)
		{
			sendTotal += sendNum;
			sendCnt++;
		}
	}
	//sprintf(buf, "stop\0");
	//sendNum = sendto(udpSocket, buf, sizeof(buf), 0, (struct sockaddr *)&addrDest, sizeof(addrDest));
	//sendTotal += sendNum;
	gettimeofday(&stop, 0);

	double timeUsed = 1000000 * (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec);
	timeUsed /= 1000;
	
	printf("Send udp data %d bytes\n", sendTotal);
	printf("Use time %.2lf ms\n", timeUsed);
	printf("Send cnt %d\n", sendCnt);
	close(udpSocket);
	printf("Terminating!\n");

	return 0;
}
