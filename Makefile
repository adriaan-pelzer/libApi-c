CFLAGS=-g -Wall -I/usr/local/include/ -I/usr/local/include/json-c -I/usr/include/curl
LIBS=-L/usr/local/lib -loauth -lJson -lCurl

all: libApi

libApi: Makefile api.o api.h addr.h
	gcc -shared -W1,-soname,libApi.so.1 -o libApi.so.1.0 api.o ${LIBS}

api.o: Makefile api.h api.c addr.h
	gcc ${CFLAGS} -fPIC -c api.c -o api.o

install:
	cp libApi.so.1.0 /usr/local/lib
	ln -sf /usr/local/lib/libApi.so.1.0 /usr/local/lib/libApi.so.1
	ln -sf /usr/local/lib/libApi.so.1.0 /usr/local/lib/libApi.so
	ldconfig /usr/local/lib
	cp api.h /usr/local/include/Api.h

clean:
	rm *.o; rm *.so*
