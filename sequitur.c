#define _GNU_SOURCE
#include <fcntl.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define	SEQUITUR_VERSION	"0.1"

typedef union {
	struct{
		int		out;		/*	read	*/
		int		in;			/*	write	*/
		}fd;
	int		p[2];
} pipe_t;

struct threads{
	pthread_t		id;
	int				num;
	pipe_t			pipe;
};


static mode_t fstype(int fd) {
	struct stat		s;

	if(fstat(fd, &s) == -1){
		exit(EXIT_FAILURE);
	}

	return s.st_mode;
}

static int consume(int in, int out, unsigned int len){
	ssize_t		written = 0;

	while (len) {
		written = splice(in, NULL, out, NULL, len, 0);
		if(written <= 0){
			fprintf(stderr, "%s\n", strerror(errno));
			return -1;
		}
		len -= written;
	}
	return 0;
}

static ssize_t mux(int in,  int *out, int nout){
	ssize_t		min = SSIZE_MAX;
	ssize_t		teed = 0;
	int			i = 0;

	while(i < nout){
		teed = tee(in, out[i], (size_t) SSIZE_MAX, SPLICE_F_NONBLOCK);
		if (teed == 0) {
			return 0;
		}
		if (teed < 0) {
			if(errno == EAGAIN){
				usleep(1000);
				continue;
			}
			fprintf(stderr, "%s\n", strerror(errno));
			return teed;
		}
		if (teed < min) {
			min = teed;
		}
		i++;
	}
	return (min == SSIZE_MAX) ? 0 : min;
}

static void usage(const char *prog){
	fprintf(stderr, "%s [OPTIONS] module [module [module]]\n", prog);
	fprintf(stderr, "   starts dynamically the given module(s)\n");
	fprintf(stderr, "   -V\tprint version and exit\n");
	fprintf(stderr, "   -h\tprints this help and exit\n");
	fprintf(stderr, "   -s\tsource to use\n");
	fprintf(stderr, "     \tif no file is given, the input is read from stdin\n");
	fprintf(stderr, "   -q\trun in quiet mode\n");
}

int main(int argc, char *argv[]){

	void			*handle;
	void			*(*func)(void *);
	struct threads	*thread;
	FILE			*sd = NULL;
	int				input = -1;
	int				origin[2];
	int				i = 0;
	int				s = 0;
	int				c = 0;
	ssize_t			processus = 0;
	int				rewrite = -1;
	int				in = -1;
	int				out = STDOUT_FILENO;
	int				*to = NULL;
	int				nto = 0;

	while((c = getopt(argc, argv, "Vhs:q")) != -1){
		switch(c){
			case 'V':
				fprintf(stdout, "sequitur version git.%s\n", VERSION);
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 's':
				sd = fopen(optarg, "r");
				if(!sd){
					fprintf(stderr, "%s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				break;
			case 'q':
				out = open("/dev/null", O_WRONLY);
				if(out < 0){
					fprintf(stderr, "%s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
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

	to = calloc(argc, sizeof(int));
	if(!to){
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

		if(pipe2(thread[i].pipe.p, O_NONBLOCK | O_CLOEXEC) < 0){
			fprintf(stderr, "%s\n", strerror(errno));
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
		to[nto++] = thread[i].pipe.fd.in;
	}


	if(sd){
		input = fileno(sd);
	} else {
		input = STDIN_FILENO;
	}

	rewrite = !S_ISFIFO(fstype(input));
	if(rewrite){
		if(pipe(origin) < 0){
			fprintf(stderr, "%s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		in = origin[0];
	} else {
		in = input;
	}

	while (1) {
		processus = mux(in, to, nto);
		if(processus < 0) {
			break;
		}
		if(processus == 0) {
			/* 	We are done	*/
			break;
		}
		if(consume(in, out, processus))
			break;
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

	if(to)
		free(to);

	if(sd)
		fclose(sd);

	if(out != STDOUT_FILENO)
		close(out);

	return EXIT_SUCCESS;
}
