#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

const char* g_server_ip = "127.0.0.1";
uint16_t g_server_port = 6666;

int do_server()
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("server socket failed: %s\n", strerror(errno));
        return -1;
    }

    uint32_t ip;
    inet_aton(g_server_ip, (struct in_addr *)&ip);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(g_server_port);

    if (bind(sock, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) < 0) {
        printf("server bind failed: %s\n", strerror(errno));
        return -1;
    }

    char buf[10240];
    struct sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    while (1) {
        ssize_t ret = recvfrom(sock, buf, sizeof(buf), 0,
                          (struct sockaddr *)&src_addr, &addrlen);
        if (ret < 0) {
            if (errno != EAGAIN) {
                printf("server recv failed: %s\n", strerror(errno));
                break;
            }
            continue;
        } 

        buf[ret] = 0;
        size_t ret_s = sendto(sock, buf, ret, 0,
                              (struct sockaddr *)&src_addr, addrlen);
        printf("resp:%s %d/%d\n", buf, ret_s, ret);
    }

    return 0;
}

void disconnect(int sock)
{
    uint32_t ip;
    inet_aton("0.0.0.0", (struct in_addr *)&ip);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    if (connect(sock, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) < 0) {
        printf("client connect failed: %s\n", strerror(errno));
    }
}

int do_client()
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("client socket failed: %s\n", strerror(errno));
        return -1;
    }

    uint32_t ip;
    inet_aton(g_server_ip, (struct in_addr *)&ip);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(g_server_port);

    char buf[10240];
    struct sockaddr_in src_addr;
    socklen_t addrlen = sizeof(src_addr);
    int i = 0;
    while (1) {
        int n = snprintf(buf, sizeof(buf), "echo %d", i++);
        size_t ret_s = sendto(sock, buf, n, 0,
                              (struct sockaddr *)&addr, sizeof(addr));
        if (ret_s != n) {
            break;
        }

        disconnect(sock);

        ssize_t ret = recvfrom(sock, buf, sizeof(buf), 0,
                          (struct sockaddr *)&src_addr, &addrlen);
        if (ret < 0) {
            if (errno != EAGAIN) {
                printf("client recv failed: %s\n", strerror(errno));
                break;
            }
            sleep(1);
            continue;
        } 

        buf[ret] = 0;
        printf("resp:%s %d/%d\n", buf, ret, n);
        sleep(1);
        break;
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
