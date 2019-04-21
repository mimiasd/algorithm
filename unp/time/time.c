#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

#define SA struct sockaddr
#define MAXLINE 1024

int main() {
	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE];
	time_t ticks;
	pid_t pid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(7777);

	bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);

	for (;;) {
		len = sizeof(cliaddr);
		connfd = accept(listenfd, (SA *)&cliaddr, &len);
		pid = fork();

		if (pid == 0) {
			ticks = time(NULL);
			snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
			write(connfd, buff, strlen(buff));
			exit(1);
		}

		if (waitpid(pid, NULL, 0) != pid) {
			printf("waitpid error");
			exit(1);
		}

		close(connfd);
	}
}
