#ifndef __COMMON__H
#define __COMMON__H
#define _POSIX_C_SOURCE 199309L
#include <sys/times.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

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
    //printf("gettimeofday[%u]\n", now);
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

unsigned int clockGettime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned int now = ts.tv_sec*1000 + ts.tv_nsec/1000000;
    //printf("clockgettime[%u]\n", now);
    return now;
}

// clock ticks to millisecond
double clockToMs(unsigned int ticks)
{
    //printf("clockpersec[%ld]\n", CLOCKS_PER_SEC);
    return (double)ticks*1000/(double)CLOCKS_PER_SEC;
}

void printMartix(double** ppMartix, int dimension)
{
    printf("\nNow the Martix is:\n");
    for(int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {
            printf("%lf ", ppMartix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printVector(double * vector, int dimension)
{
    printf("\nNow the vector is:\n");
    for(int i = 0; i < dimension; i++)
    {
        printf("%lf ", vector[i]);
    }
    printf("\n");
}


#endif
