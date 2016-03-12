#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

const int PORT = 9876;
const char IP_DEST[10] = "127.0.0.1";

//Time Subtract
/*
 * int timeSub(struct timeval *result, struct timeval *start, struct timeval *stop)
{
	int nsec;
	if(x->tv_sec > y->tv_sec)
		return -1;
	if((x->tv_sec == y->tv_sec) && (x->tv_usec > y->tv_usec))
		return -1;
	result->tv_sec = (y->tv_sec - x->tv_sec);
	result->tv_usec = (y->tv_usec - x->tv_usec);
	if(result->tv_usec < 0)
	{
		result->tv_sec--;
		result->tv_usec += 1000000;
	}
	return 0;
}
*/
int main()
{
	//Time Test
	struct timeval start, stop, diff;

	//Socket Buffer Maxinum
	int rcv_size = 0;
	int snd_size = 0;
	int err = -1;
	socklen_t optlen;

	char buf[1024];
	struct sockaddr_in addrSrc;
	memset((void*)&addrSrc, 0, sizeof(addrSrc));
	addrSrc.sin_family = AF_INET;
	addrSrc.sin_port = htons(PORT);
	addrSrc.sin_addr.s_addr = htonl(INADDR_ANY);
	//addrSrc.sin_addr.s_addr = inet_addr(IP_DEST);
	socklen_t addrSrc_len = sizeof(addrSrc);

	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPv4 UDP
	if(udpSocket < 0)
	{
		printf("Create UDP Socket Error!\n");
		return -1;
	}

	optlen = sizeof(rcv_size);
	err = getsockopt(udpSocket, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);
	if(err < 0)
	{
		printf("Get Recv Buff Size Error!\n");
		return -1;
	}
	printf("System Socket Recv Buff Size: %d Bytes\n", rcv_size);
	
	optlen = sizeof(snd_size);
	err = getsockopt(udpSocket, SOL_SOCKET, SO_SNDBUF, &snd_size, &optlen);
	if(err < 0)
	{
		printf("Get Send Buff Size Error!\n");
		return -1;
	}
	printf("System Socket Send Buff Size: %d Bytes\n", snd_size);
	
	rcv_size = 80 * 1024;
	optlen = sizeof(rcv_size);
	err = setsockopt(udpSocket, SOL_SOCKET, SO_RCVBUF, &rcv_size, optlen);
	if(err < 0)
	{
		printf("Set Recv Buff Size Error!\n");
		return -1;
	}
	optlen = sizeof(rcv_size);
	err = getsockopt(udpSocket, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);
	if(err < 0)
	{
		printf("Get Recv Buff Size(My) Error!\n");
		return -1;
	}
	printf("System Socket Recv Buff Size(My): %d Bytes\n", rcv_size);
	
	snd_size = 80 * 1024;
	optlen = sizeof(snd_size);
	err = setsockopt(udpSocket, SOL_SOCKET, SO_SNDBUF, &snd_size, optlen);
	if(err < 0)
	{
		printf("Set Send Buff Size Error!\n");
		return -1;
	}
	optlen = sizeof(snd_size);
	err = getsockopt(udpSocket, SOL_SOCKET, SO_SNDBUF, &snd_size, &optlen);
	if(err < 0)
	{
		printf("Get Send Buff Size(My) Error!\n");
		return -1;
	}
	printf("System Socket Send Buff Size(My): %d Bytes\n", snd_size);

	int result = bind(udpSocket, (struct sockaddr*)&addrSrc, sizeof(addrSrc));
	if(result < 0)
	{
		printf("Bind UDP Socket Error!\n");
		return -1;
	}
	printf("Waiting for message:\n");
	int recvTotal = 0;
	int recvNum = 0;
	int packCnt = 0;
	while(1)
	{
		recvNum = recvfrom(udpSocket, buf, sizeof(buf), 0, (struct sockaddr*)&addrSrc, &addrSrc_len);
		if(recvTotal == 0)
			gettimeofday(&start, 0);
		recvTotal += recvNum;
		packCnt++;
		//printf("Received from client:%s\n", buf);
		if(strncmp(buf, "stop", 4) == 0) // End Receive
		{
			printf("Sender told me to end the connection!\n");
			break;
		}
	}
	gettimeofday(&stop, 0);
	printf("Get %d bytes from udp client\n", recvTotal);
	printf("Get %d packages\n", packCnt);
	//timeSub(&diff, &start, &stop);
	//printf("Use time: %dms\n", diff.tv_usec);
	double timeUsed = 1000000 * (stop.tv_sec - start.tv_sec) + (stop.tv_usec - start.tv_usec); //microsecond
	timeUsed /= 1000; //millisecond
	printf("Use time: %.2lf ms\n", timeUsed);
	
	close(udpSocket);
	return 0;
}
