CCFLAGS    += -g -Wall -Wextra -std=gnu99
CCSTATIC   +=${CCFLAGS} -rdynamic
CCDYNAMIC  +=${CCFLAGS} -fPIC

LDFLAGS    += -lpthread
LDSTATIC   +=${LDFLAGS} -ldl -lpopt
LDDYNAMIC  +=${LDFLAGS} -shared


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
