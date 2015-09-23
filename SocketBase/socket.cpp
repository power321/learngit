#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 5555
#define BUF_SIZE 1024 * 4
#define MAX_LISTEN_NUM 5

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    memset((void *)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket < 0)
    {
        printf("create socket error!\n");
        return -1;
    }

    int bindRet = bind(listenSocket, (struct sockaddr *)&addr, sizeof(addr));
    if(bindRet < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    int listenRet = listen(listenSocket, MAX_LISTEN_NUM);
    if(listenRet < 0)
    {
        printf("listen error!\n");
        return -1;
    }
    return 0;
}
