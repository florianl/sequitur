#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

int main(int argc, char *argv[]){

	void			*handle;
	int				(*func)();


	handle = dlopen("./exemplum.so", RTLD_NOW);

	if (!handle){
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	dlerror();

	func = (int (*)()) dlsym(handle,"func");
	if(!func) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	(*func)(argc, argv);

	printf("functions returns %i\n",(*func)());

	dlerror();

	return EXIT_SUCCESS;
}
