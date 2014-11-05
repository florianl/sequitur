#include <stdlib.h>
#include <stdio.h>

int func(int argc, char *argv[]){

	int	i;

	for(i=0; i<argc; i++)
		fprintf(stderr, "%s\n", argv[i]);

	return EXIT_SUCCESS;
}
