#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct threads{
	pthread_t		id;
	int				num;
	int				pipeline[2];
};

int func(void *arg){

	struct threads	*thread = arg;
	char buf;

	close(thread->pipeline[1]);

	fprintf(stdout, "Hello World from %d\n", thread->num);

	while(read(thread->pipeline[0], &buf, 1) > 0)
		fprintf(stdout, "%c", buf);
	fprintf(stdout, "\n");
	close(thread->pipeline[0]);

	return EXIT_SUCCESS;
}
