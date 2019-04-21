#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	fd_set rd;
	struct timeval tv;
	int err;

	FD_ZERO(&rd);
	FD_SET(0, &rd);
	printf("sizeof fd_set: %d\n", sizeof(fd_set));

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	err = select(0, &rd, NULL, NULL, &tv);

	if (err == 0) {
		printf("select time out!\n");
	} else if (err == -1) {
		printf("fail to select!\n");
	} else {
		printf("data is available!\n");
	}

	return 0;
}
