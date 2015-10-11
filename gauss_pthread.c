/*  Author by Zhen Shao(zshao5@hawk.iit.edu).
    Time: 10/10/2015.
    This file is part of hw2 of cs546.
    Purpose of file is to domenstrate the efficiency of parallel gauss elimination
    without privot algorithm using pthread.

    Usage:
        Run ./gauss_pthread you would get the correct vector result and time consumer description

    Algorithm description:
        Since the outer loop has data dependence in each iterations,
        We would try two data split method: one is for the middle loop and another one is the inner loop
        We also use conditional to synchorize threads
*/
#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "common.h"

#define min(a, b) ((a)<(b)?(a):(b))

// global variable
const int MAX_MATRIX_VALUE = 1000;
typedef double EleType;
EleType** ppMartix = NULL;
EleType* pVecB = NULL;
EleType* pVecX = NULL;
int dimension = 0;  // matrix dimension
int threadnum = 10;  // parallel thread num





// use rand number to initilize the matrix and vector
void initMatrix()
{
    assert(dimension > 0);
    setRandSeed(time(NULL));

    ppMartix = (EleType**)calloc(dimension, sizeof(EleType*));
    assert(ppMartix != NULL);
    for(int i = 0; i < dimension; i++)
    {
        ppMartix[i] = (EleType*)calloc(dimension, sizeof(EleType));
        assert(ppMartix[i] != NULL);
    }

    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++)
            ppMartix[i][j] = getRandNum(MAX_MATRIX_VALUE);

    pVecB = (EleType*)calloc(dimension, sizeof(EleType));
    assert(pVecB != NULL);
    for(int i = 0; i < dimension; i++)
        pVecB[i] = getRandNum(MAX_MATRIX_VALUE);

    pVecX = (EleType*)calloc(dimension, sizeof(EleType));
    assert(pVecX != NULL);
}


#include <pthread.h>

pthread_mutex_t barrierMutex;
pthread_cond_t barrierCond;
int arriveCount = 0;

void barrier()
{
    pthread_mutex_lock(&barrierMutex);
    arriveCount++;

    if(arriveCount < threadnum)
    {
        pthread_cond_wait(&barrierCond, &barrierMutex);
    }
    if(arriveCount >= threadnum)
    {
        pthread_cond_broadcast(&barrierCond);
        arriveCount = 0;
    }

    pthread_mutex_unlock(&barrierMutex);
}

// the parameter id should start from 0
void* gauss_elimination_pthread_middleloop(void* id)
{
    for(int column = 0; column< dimension -1; column++)
    {
        // here we parallel the loop based on the tid
        int tid = *(int*)(&id);
	printf("tid:%d\n", tid);
        int totalRow = dimension - column - 1;
        int from = totalRow*tid/threadnum + column + 1;
        int to = (int)min(totalRow*(tid+1)/threadnum, dimension);
        for(int row = from; row < to; row++)
        {
            EleType fac = ppMartix[row][column] / ppMartix[column][column];
            for(int index = column; index < dimension; index++)
            {
                ppMartix[row][index] -= fac*ppMartix[column][index];
            }
            pVecB[row] -= fac*pVecB[column];
        }

        barrier();
    }

    return 0;
}

void gauss_back_substitution()
{
    for(int index = dimension - 1; index >= 0; index--)
    {
        pVecX[index] = pVecB[index];
        for(int i = dimension - 1; i > index; i--)
        {
            pVecX[index] -= pVecX[i]*ppMartix[index][i];
        }
        pVecX[index] /= ppMartix[index][index];
    }

}

void gauss()
{
    pthread_mutex_init(&barrierMutex, NULL);
    pthread_cond_init(&barrierCond, NULL);
    arriveCount = 0;

    pthread_t* ppt = calloc(threadnum, sizeof(pthread_t));
    assert(ppt != NULL);
    for(int i = 0; i < threadnum; i++)
        pthread_create(&ppt[i], NULL, gauss_elimination_pthread_middleloop, (void*)i);

    for(int i = 0; i < threadnum; i++)
        pthread_join(ppt[i], NULL);

    gauss_back_substitution();
}

// should contain two parameter
// first one is: dimension
// second one is: threadnum
void getInput(int argc, char** argv)
{
    assert(argc == 3);
    dimension = atoi(argv[1]);
    threadnum = atoi(argv[2]);
}

int main(int argc, char* argv[])
{
    getInput(argc, argv);

    initMatrix();

    clock_t begin = getClock();
    clock_t begin_unix = getClock_unix();
    unsigned int begin_time = getTime();
    unsigned int clockgettime_b = clockGettime();
    gauss();
    clock_t end = getClock();
    clock_t end_unix = getClock_unix();
    unsigned int end_time = getTime();
    unsigned int clockgettime_e = clockGettime();

    double time_elapse = clockToMs(end-begin);
    double time_elapse_unix = clockToMs(end_unix - begin_unix);

    printf("Pthread Matrix dimension[%d] threadnum[%d] cost clocktime[%lf]ms gettimeofday[%u]ms clockgettime[%u]ms\n", dimension, threadnum, time_elapse, end_time-begin_time, clockgettime_e - clockgettime_b);
    return 0;
}










