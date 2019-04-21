#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/resource.h>

#define MAXEPOLL 10
#define MAXLINE 1024
#define PORT 6000
#define MAXBACK 1000

int setnonblocking(int fd) {
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK) == -1) {
		printf("set blocking error: %d\n", errno);
		return -1;
	}
	
	return 0;
}

int main(int argc, char ** argv) {
	int listen_fd, conn_fd, epoll_fd;
	int nread;
	int cur_fds;
	int wait_fds;
	int i;
	struct sockaddr_in servaddr;
	struct sockaddr_in cliaddr;
	struct epoll_event ev;
	struct epoll_event evs[MAXEPOLL];
	struct rlimit rlt;
	char buf[MAXLINE];
	socklen_t len = sizeof(struct sockaddr_in);

	rlt.rlim_max = rlt.rlim_cur = MAXEPOLL;
	if (setrlimit(RLIMIT_NOFILE, &rlt) == -1) {
		printf("setrlimit error: %d\n", errno);
		exit(EXIT_FAILURE);
	} 

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		printf("socket error: %d...\n", errno);
		exit(EXIT_FAILURE);
	}

	if (setnonblocking(listen_fd) == -1) {
		printf("setbnonblocking error: %d...\n", errno);
		exit(EXIT_FAILURE);
	}

	if (bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1) {
		printf("bind error: %d...\n", errno);
		exit(EXIT_FAILURE);
	}

	if (listen(listen_fd, MAXBACK) == -1) {
		printf("listen error: %d...\n", errno);
		exit(EXIT_FAILURE);
	}

	epoll_fd = epoll_create(MAXEPOLL);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listen_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
		printf("epoll error: %d...\n", errno);
		exit(EXIT_FAILURE);
	}
	cur_fds = 1;

	for (;;) {
		if ((wait_fds = epoll_wait(epoll_fd, evs, cur_fds, -1)) == -1) {
			printf("epoll wait error: %d...\n", errno);
			exit(EXIT_FAILURE);
		}

		printf("wait_fds: %d\n", wait_fds);

		for (i = 0; i < wait_fds; i++) {
			if (evs[i].data.fd == listen_fd && cur_fds < MAXEPOLL) {
				if ((conn_fd = accept(listen_fd, (struct sockaddr *)&cliaddr, &len)) == -1) {
					printf("accept error: %d...\n", errno);
					exit(EXIT_FAILURE);
				}
				printf("server get from client, connection fd: %d !\n", conn_fd);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0) {
					printf("epoll error: %d...\n", errno);
					exit(EXIT_FAILURE);
				}
				++cur_fds;
				continue;
			}

			printf("current fds: %d !\n", cur_fds);

			printf("read from %d fd: %d !\n", i, evs[i].data.fd);
			printf("read from %d+1 fd: %d !\n", i, evs[i+1].data.fd);
			printf("read from %d+2 fd: %d !\n", i, evs[i+2].data.fd);
			printf("read from %d+3 fd: %d !\n", i, evs[i+3].data.fd);
			printf("read from %d+4 fd: %d !\n", i, evs[i+4].data.fd);
			printf("read from %d+5 fd: %d !\n", i, evs[i+5].data.fd);
			printf("read from %d+6 fd: %d !\n", i, evs[i+6].data.fd);
			printf("read from %d+7 fd: %d !\n", i, evs[i+7].data.fd);
			printf("read from %d+8 fd: %d !\n", i, evs[i+8].data.fd);
			printf("read from %d+9 fd: %d !\n", i, evs[i+9].data.fd);

			nread = read(evs[i].data.fd, buf, sizeof(buf));
			if (nread <= 0) {
				close(evs[i].data.fd);
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
				--cur_fds;
				continue;
			}

			write(evs[i].data.fd, buf, nread);
		}
	}

	close(listen_fd);
	return 0;
}
