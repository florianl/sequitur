#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

struct threads{
	pthread_t		id;
	int				num;
};

int main(int argc, char *argv[]){

	void			*handle;
	void			*(*func)(void *);
	struct threads	*thread;
	int				errno;
	int				i = 0;
	int				s = 0;

	thread = calloc(argc+1, sizeof(struct threads));
	if(!thread){
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for(i=0; i<=argc; i++){
		thread[i].num = i+1;

		handle = dlopen("./exemplum.so", RTLD_NOW);

		if(!handle){
			fprintf(stderr, "%s\n", dlerror());
			exit(EXIT_FAILURE);
		}

		dlerror();

		func = (void *(*)()) dlsym(handle,"func");
		if(!func){
			fprintf(stderr, "%s\n", dlerror());
			exit(EXIT_FAILURE);
		}

		s = pthread_create(&thread[i].id, NULL, &(*func), &thread[i]);
		if(s!=0){
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	for(i=0; i<=argc; i++){
		s = pthread_join(thread[i].id, NULL);
		if(s!=0){
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if(thread)
		free(thread);

	return EXIT_SUCCESS;
}
