#include <stdlib.h>
#include <stdio.h>

struct threads{
	pthread_t		id;
	int				num;
};

int func(void *arg){

	struct threads	*thread = arg;

	fprintf(stdout, "Hello World from %d\n", thread->num);

	return EXIT_SUCCESS;
}
