

PTHREAD_TARGET=gauss_pthread
PTHREAD_OBJ = gauss_pthread.o

TEST_TARGET=gauss_test
TEST_OBJ = gauss_test.o

NORMAL_TARGET=gauss_normal
NORMAL_OBJ = gauss_normal.o

OPENMP_TARGET = gauss_openmp
OPENMP_SRC = gauss_openmp.c
OPENMP_OBJ = gauss_openmp.o

REBUILDABLE = *.o $(OPENMP_TARGET) $(PTHREAD_TARGET) $(TEST_TARGET) $(NORMAL_TARGET)

CFLAGS := -g -Wall -DDEBUG -std=c99 -Wno-unused-variable -lrt

all:$(PTHREAD_TARGET) $(TEST_TARGET) $(NORMAL_TARGET) $(OPENMP_TARGET)

$(TEST_TARGET):$(TEST_OBJ)
	gcc -o $(TEST_TARGET) $(TEST_OBJ) $(CFLAGS)

$(PTHREAD_TARGET):$(PTHREAD_OBJ)
	gcc -o $(PTHREAD_TARGET) $(PTHREAD_OBJ) $(CFLAGS) -lpthread

$(NORMAL_TARGET):$(NORMAL_OBJ)
	gcc -o $(NORMAL_TARGET) $(NORMAL_OBJ) $(CFLAGS)

$(OPENMP_TARGET):$(OPENMP_SRC)
	gcc -o $(OPENMP_TARGET) $(OPENMP_SRC) $(CFLAGS) -fopenmp

clean:
	rm -rf $(REBUILDABLE)
