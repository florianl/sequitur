CCFLAGS    += -g -Wall -Wextra -std=gnu99 -DVERSION=\"$(GIT_VERS)\"
CCSTATIC   +=${CCFLAGS} -rdynamic
CCDYNAMIC  +=${CCFLAGS} -fPIC

LDFLAGS    += -lpthread
LDSTATIC   +=${LDFLAGS} -ldl
LDDYNAMIC  +=${LDFLAGS} -shared

GIT_VERS   := $(shell git describe --abbrev=6 --dirty --always --tags)

OBJECTS = exemplum.o exemplum2.o

all: sequitur exemplum.so exemplum2.so

%.o: %.c
	${CC} ${CCDYNAMIC} -c -o $@ $<

sequitur.o: sequitur.c
	${CC} ${CCSTATIC} -c -o $@ $<

%.so: %.o
	${CC} ${LDDYNAMIC} -o $@ $<

sequitur: sequitur.o
	${CC} ${LDSTATIC} -o $@ $< 

clean: 
	-rm -f *.o *.so sequitur
