#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    const char *ip = "127.0.0.1";
    int port = 9981;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);
    int ret = connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret == 0) {
        printf("connect success\n");
    }
    char buf[1000] = {0};
    for (int i = 0; i < 1000; ++i) {
        buf[i] = 'a';
    }
    while (getchar()) {
        ret = send(sock, buf, 100, 0);
        if (ret < 0) {
            printf("send failure\n");
        }
        else {
            printf("send %d bytes\n", ret);
        }
    }
    close(sock);
}
