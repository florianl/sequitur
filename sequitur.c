#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define	SEQUITUR_VERSION	"0.1"

struct threads{
	pthread_t		id;
	int				num;
};

static void usage(const char *prog){
	fprintf(stderr, "%s [OPTIONS] module [module [module]]\n", prog);
	fprintf(stderr, "   starts dynamically the given module(s)\n");
	fprintf(stderr, "   -V\tprint version and exit\n");
	fprintf(stderr, "   -h\tprints this help and exit\n");
}

int main(int argc, char *argv[]){

	void			*handle;
	void			*(*func)(void *);
	struct threads	*thread;
	int				errno;
	int				i = 0;
	int				s = 0;
	int				c = 0;

	while((c = getopt(argc, argv, "Vh")) != -1){
		switch(c){
			case 'V':
				fprintf(stdout, "sequitur version %s\n", SEQUITUR_VERSION);
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			default:
				break;
		}
	}

	argc -= optind;
	argv += optind;

	if(argc < 1){
		fprintf(stderr, "missing module(s) to load\n");
		exit(EXIT_FAILURE);
	}

	thread = calloc(argc, sizeof(struct threads));
	if(!thread){
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	for(i=0; i<argc; i++){
		thread[i].num = i+1;

		handle = dlopen(argv[i], RTLD_NOW);

		if(!handle){
			fprintf(stderr, "%s\n", dlerror());
			continue;
		}

		dlerror();

		func = (void *(*)()) dlsym(handle,"func");
		if(!func){
			fprintf(stderr, "%s\n", dlerror());
			continue;
		}

		s = pthread_create(&thread[i].id, NULL, &(*func), &thread[i]);
		if(s!=0){
			fprintf(stderr, "%s\n", strerror(errno));
			continue;
		}
	}

	for(i=0; i<argc; i++){
		if(thread[i].id==0)
			continue;
		s = pthread_join(thread[i].id, NULL);
		if(s!=0){
			fprintf(stderr, "%s\n", strerror(errno));
			continue;
		}
	}

	if(thread)
		free(thread);

	return EXIT_SUCCESS;
}
