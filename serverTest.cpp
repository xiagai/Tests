#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/epoll.h>
#include <stdio.h>

int main() {
    const char *ip = "127.0.0.1";
    int port = 9981;

    int serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);

    int ret = bind(serverfd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    assert(ret != -1);

    ret = listen(serverfd, 5);
    assert(ret != -1);
    
    int epollfd = epoll_create1(0);
    epoll_event server_event;
    server_event.data.fd = serverfd;
    server_event.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, serverfd, &server_event);
    
    int serverfd2 = dup(serverfd);
    close(serverfd);
    epoll_event ret_event[1000];
    while (1) {
        int num = epoll_wait(epollfd, ret_event, 1000, 3);
        if (num != 0) {
            printf("%d events ready\n", num);
            sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof clientAddr;
            accept(serverfd, (sockaddr *)&clientAddr, &clientAddrLen);
        }
    }

    close(serverfd);
    return 0;
}
    