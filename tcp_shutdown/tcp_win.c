#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
/*#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
*/
#include "winsock2.h"
#include "winsock.h"
#include "windows.h"

#pragma comment(lib, "ws2_32.lib")


const char* g_server_ip = "127.0.0.1";
uint16_t g_server_port = 6666;

void server_process(int sock)
{
    char buf[10240];

    while (1) {
        ssize_t ret = recv(sock, buf, sizeof(buf), 0);
        if (ret < 0) {
            if (errno != EAGAIN) {
                printf("server recv failed: %s\n", strerror(errno));fflush(stdout);
                break;
            }
            continue;
        } 

        if (ret == 0) {
            printf("read end!\n");fflush(stdout);
            break;
        }

        buf[ret] = 0;
        //int i = 0;
        //for (; i < 3; i++) {
            size_t ret_s = send(sock, buf, ret, 0);
            printf("resp:%s %d/%d\n", buf, ret_s, ret);fflush(stdout);
        //}
    }
}

int do_server()
{
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  
        printf("Failed. Error Code : %d\n", WSAGetLastError());  fflush(stdout);
        return -1;  
    }
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("server socket failed: %s\n", strerror(errno));fflush(stdout);
        //return -1;
    }

    uint32_t ip = inet_addr(g_server_ip);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(g_server_port);

    if (bind(sock, (struct sockaddr *)&addr, (int)sizeof(addr)) < 0) {
        printf("server bind failed: %s\n", strerror(errno));fflush(stdout);
        //return -1;
    }

    if (listen(sock, 10) < 0) {
        printf("listen failed: %s\n", strerror(errno));fflush(stdout);
        //return -1;
    }

    while (1) {
        int new_sock = accept(sock, NULL, NULL);
        if (new_sock < 0) {
            continue;
        }

        server_process(new_sock);
        close(new_sock);
    }

    return 0;
}

int do_client()
{
	WSADATA wsaData;
	SOCKET sock;
	uint32_t ip;
	struct sockaddr_in addr;
	char buf[10240];
    int i = 0;
	
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  
        printf("Failed. Error Code : %d\n", WSAGetLastError()); fflush(stdout); 
        return -1;  
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("client socket failed: %s\n", strerror(errno));fflush(stdout);
        return -1;
    }

    ip = inet_addr(g_server_ip);
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(g_server_port);

    if (connect(sock, (struct sockaddr *)&addr, (int)sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }

    while (1) {
        int n = snprintf(buf, sizeof(buf), "echo %d", i++);
        size_t ret_s = send(sock, buf, n, 0);
        if (ret_s != n) {
			printf("send: %d\n", ret_s);fflush(stdout);
            break;
        }

		//SD_RECEIVE，SD_SEND
        if (shutdown(sock, SD_RECEIVE) < 0) {
            printf("shutdown failed: %s\n", strerror(errno));fflush(stdout);
        } 

        /*ssize_t ret = recv(sock, buf, sizeof(buf), 0);
        if (ret < 0) {
            if (errno != EAGAIN) {
                printf("client recv failed: %s\n", strerror(errno));fflush(stdout);
                break;
            }
            Sleep(1000);
            continue;
        }*/
        //Sleep(1000);
        //continue;

        // close(sock);
        //buf[ret] = 0;
        //printf("resp:%s %d/%d\n", buf, ret, n);fflush(stdout);
        Sleep(10000);
        //break;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s [opt]\n", argv[0]);
        printf("       -s    as server listen on %s:%d\n", g_server_ip, g_server_port);
        printf("       -c    as client\n");
        return 0;
    }

    if (strncmp(argv[1], "-s", strlen("-s")) == 0) {
        do_server();
        return 0;
    }

    do_client();
    return 0;
}
