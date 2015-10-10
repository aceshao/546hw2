

PTHREAD_TARGET=gauss_pthread
PTHREAD_SRC = gauss_pthread.c

REBUILDABLE = *.o $(PTHREAD_TARGET)

CFLAGS := -g -Wall -DDEBUG -std=c99

all:$(PTHREAD_TARGET)

clean:
	rm -rf $(REBUILDABLE)

$(PTHREAD_TARGET):
	gcc -o $(PTHREAD_TARGET) $(PTHREAD_SRC) $(CFLAGS) -lpthread 

