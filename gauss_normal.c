/*  Author by Zhen Shao(zshao5@hawk.iit.edu).
    Time: 10/10/2015.
    This file is part of hw2 of cs546.
    Purpose of file is to domenstrate the efficiency of gauss elimination
    without privot algorithm by sequenal.

    Usage:
        Run ./gauss_normal you would get the correct vector result and time consumer description

    Algorithm description:
        SThe Gauss-Elimination-without-privot algorithm is first to get the upper-triangle matrix
        and then do back substitution. The time complexity is O(n3)+O(n2) = O(n3).
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "common.h"

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

void* gauss_elimination()
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
    gauss_elimination();
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

    printf("Sequenal Matrix dimension[%d] threadnum[%d] cost clocktime[%lf]ms gettimeofday[%u]ms\n", dimension, threadnum, time_elapse, end_time-begin_time);
    return 0;
}










