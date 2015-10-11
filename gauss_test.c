/*  Author by Zhen Shao(zshao5@hawk.iit.edu).
    Time: 10/10/2015.
    This file is part of hw2 of cs546.
    Purpose of file is to domenstrate correctness of gauss elimination
    without privot algorithm

    Usage:
        You can initiate the original matrix and vector in the config file 
        "gtest.dat". and run ./gauss_test you would get the correct vector result.

    Algorithm description:
        The Gauss-Elimination-without-privot algorithm is first to get the upper-triangle matrix
        and then do back substitution. The time complexity is O(n3)+O(n2) = O(n3).
*/

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "common.h"

void PRINTF(const char* content)
{
    printf("%s\n", content);
}

// global variable
typedef double EleType;
const char* filename = "gtest.dat";
EleType** ppMartix = NULL;
EleType* pVecB = NULL;
EleType* pVecX = NULL;
int dimension = 0;

// from a config file we read in the matrix A
// also the b which is the result vector
// and initialize the x which is the solution vector
// thus we wanna the solution A*x = b
int readMatrix(const char* filename)
{
    FILE * fid = fopen(filename, "r");
    assert(fid != NULL);

    dimension = 0;
    fscanf(fid, "%d", &dimension);
    assert(dimension > 0);

    ppMartix = (EleType**)calloc(dimension, sizeof(EleType*));
    assert(ppMartix != NULL);
    for(int i = 0; i < dimension; i++)
    {
        ppMartix[i] = (EleType*)calloc(dimension, sizeof(EleType));
        assert(ppMartix[i] != NULL);
    }

    for(int i = 0; i < dimension; i++)
        for(int j = 0; j < dimension; j++)
            fscanf(fid, "%lf", &ppMartix[i][j]);

    pVecB = (EleType*)calloc(dimension, sizeof(EleType));
    assert(pVecB != NULL);
    for(int i = 0; i < dimension; i++)
        fscanf(fid, "%lf", &pVecB[i]);

    pVecX = (EleType*)calloc(dimension, sizeof(EleType));
    assert(pVecX != NULL);

    return 0;
}

// given the matrix, the vector b and dimension, we calculate the vector x and store it in the pvecx
void gauss(EleType **ppmartix, EleType *pvecb, EleType *pvecx, int dimension)
{
    // get the upper-triangular matrix
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

    // now begin the back substitution
    for(int index = dimension - 1; index >= 0; index--)
    {
        pvecx[index] = pvecb[index];
        for(int i = dimension - 1; i > index; i--)
        {
            pvecx[index] -= pvecx[i]*ppmartix[index][i];
        }
        pvecx[index] /= ppmartix[index][index];
    }

    printVector(pvecx, dimension);

}

void test_normal()
{
    readMatrix(filename);

    printMartix(ppMartix, dimension);

    printVector(pVecB, dimension);

    gauss(ppMartix, pVecB, pVecX, dimension);
}

int main()
{
    test_normal();

}
