/*  Author by Zhen Shao(zshao5@hawk.iit.edu).
    Time: 10/10/2015.
    This file is part of hw2 of cs546.
    Purpose of file is to domenstrate the efficiency of parallel gauss elimination
    without privot algorithm using openmp.

    Usage:
        Run ./gauss_openmp you would get the correct vector result and time consumer description

    Algorithm description:
        Since the outer loop has data dependence in each iterations,
        We would try two data split method: one is for the middle loop and another one is the inner loop
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

// global variable
const int MAX_MATRIX_VALUE = 1000;
typedef double EleType;
EleType** ppMartix = NULL;
EleType* pVecB = NULL;
EleType* pVecX = NULL;
int dimension = 0;  // matrix dimension
int threadnum = 10;  // parallel thread num


void setRandSeed(unsigned int seed)
{
    srand(seed);
}            

int getRandNum(int maxnum)
{
    assert(maxnum > 0);
    return rand()%maxnum+1;
}

unsigned int getTime()
{
    struct timeval temp;
    gettimeofday(&temp, NULL);
    unsigned int now = temp.tv_sec*1000 + temp.tv_usec/1000;
//    printf("gettimeofday[%ld]\n", now);
    return now;
}

clock_t getClock_unix()
{
    struct tms temp;
    times(&temp);
    return temp.tms_utime + temp.tms_stime;
}

clock_t getClock()
{
    return clock();
}

// clock ticks to millisecond
double clockToMs(unsigned int ticks)
{
    return (double)ticks*1000/(double)CLOCKS_PER_SEC;
}


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

// the parameter id should start from 0
void* gauss_elimination_openmp_middleloop()
{
    for(int column = 0; column< dimension -1; column++)
    {
        int from = column + 1;
        int to = dimension;
        for(int row = from; row < to; row++)
        {
            EleType fac = ppMartix[row][column] / ppMartix[column][column];
            for(int index = column; index < dimension; index++)
            {
                ppMartix[row][index] -= fac*ppMartix[column][index];
            }
            pVecB[row] -= fac*pVecB[column];
        }
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
    gauss_elimination_openmp_middleloop();
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
    gauss();
    clock_t end = getClock();
    clock_t end_unix = getClock_unix();
    unsigned int end_time = getTime();

    double time_elapse = clockToMs(end-begin);
    double time_elapse_unix = clockToMs(end_unix - begin_unix);

    printf("Openmp Matrix dimension[%d] threadnum[%d] cost clocktime[%lf]ms gettimeofday[%ld]\n", dimension, threadnum, time_elapse, end_time-begin_time);
    return 0;
}














void gauss(EleType **ppmartix, EleType *pvecb, EleType *pvecx, int dimension)
{
    for(int column = 0; column< dimension -1; column++)
    {
        for(int row = column + 1; row < dimension; row++)
        {
            EleType fac = ppmartix[row][column] / ppmartix[column][column];
            for(int index = column; index < dimension; index++)
            {
                ppmartix[row][index] -= fac*ppmartix[column][index];
            }
            pvecb[row] -= fac*pvecb[column];
        }
    }

    for(int index = dimension - 1; index >= 0; index--)
    {
        pvecx[index] = pvecb[index];
        for(int i = dimension - 1; i > index; i--)
        {
            pvecx[index] -= pvecx[i]*ppmartix[index][i];
        }
        pvecx[index] /= ppmartix[index][index];
    }

}