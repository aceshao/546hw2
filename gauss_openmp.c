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