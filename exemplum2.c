#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef struct{
		int		out;
		int		in;
}pipe_t;

struct threads{
	pthread_t		id;
	int				num;
	pipe_t			pipe;
};

int func(void *arg){

	struct threads	*thread = arg;
	char buf;
	ssize_t		len = 0;

	fprintf(stdout, "Hello World from %d\n", thread->num);

	do{
		len = read(thread->pipe.out, &buf, 1);
		if(len < 0){
			if (errno == EAGAIN) {
					usleep(1000);
					continue;
				}
			fprintf(stderr, "%s():%d\t%s\n", __func__, __LINE__, strerror(errno));
			break;
		}
		if(len == 0){
			/* 	We are done	*/
			break;
		}
		fprintf(stdout, "[%d] \\x%02x\n", thread->num, buf);
	} while(1);

	return EXIT_SUCCESS;
}
