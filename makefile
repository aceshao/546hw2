

PTHREAD_TARGET=gauss_pthread
PTHREAD_SRC = gauss_pthread.c

TEST_TARGET=gauss_test
TEST_SRC = gauss_test.c

NORMAL_TARGET=gauss_normal
NORMAL_SRC = gauss_normal.c

REBUILDABLE = *.o $(PTHREAD_TARGET) $(TEST_TARGET) $(NORMAL_TARGET)

CFLAGS := -g -Wall -DDEBUG -std=c99 -Wno-unused-variable

all:$(PTHREAD_TARGET) $(TEST_TARGET) $(NORMAL_TARGET)

$(TEST_TARGET):
	gcc -o $(TEST_TARGET) $(TEST_SRC) $(CFLAGS)

$(PTHREAD_TARGET):
	gcc -o $(PTHREAD_TARGET) $(PTHREAD_SRC) $(CFLAGS) -lpthread

$(NORMAL_TARGET):
	gcc -o $(NORMAL_TARGET) $(NORMAL_SRC) $(CFLAGS)

clean:
	rm -rf $(REBUILDABLE)
