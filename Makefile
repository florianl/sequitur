CCFLAGS    += -g -Wall
CCSTATIC   +=${CCFLAGS} -rdynamic
CCDYNAMIC  +=${CCFLAGS} -fPIC

LDFLAGS    += -lpthread
LDSTATIC   +=${LDFLAGS} -ldl -lpopt
LDDYNAMIC  +=${LDFLAGS} -shared


OBJECTS = exemplum.o

all: sequitur exemplum.so

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
