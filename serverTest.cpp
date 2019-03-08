// 可以通过close fd的方式来对epoll中的fd进行反注册，但要注意：
// 如果epoll注册的fd所指文件有多个fd同时指向（因为dup或fork等原因），则需要close所有fd，才能反注册成功，否则epoll仍能监听到事件并返回在就绪列表中。如果你只close了epoll监听的那个fd，你将无法通过fd来处理该事件，甚至无法通过EPOLL_CTL_DEL反注册该事件。
// 因此，最好是在close前，就EPOLL_CTL_DEL该事件。

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
    
