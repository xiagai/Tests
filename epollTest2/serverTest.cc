// ET模式下，只是一次发送的数据不会同时触发，如果对方多次发送，仍会触发！

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

    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof clientAddr;
    int clientfd = accept(serverfd, (sockaddr *)&clientAddr, &clientAddrLen);

    int epollfd = epoll_create1(0);
    epoll_event clientEvent;
    clientEvent.data.fd = clientfd;
    clientEvent.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &clientEvent);

    epoll_event ret_event[1000];
    while (1) {
        int num = epoll_wait(epollfd, ret_event, 1000, 3);
        if (num != 0) {
            printf("%d events ready\n", num);
            char buf[100] = {0};
            size_t n = recv(clientfd, &buf, 50, 0);
            printf("%d bytes received.\n", n);
        }
    }

    close(serverfd);
    return 0;
}
    
