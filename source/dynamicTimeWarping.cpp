#include <iostream>
#include <string>

#include "dataStructs.hpp"
#include "fileReading.hpp"
#include "metrics.hpp"
#include "dynamicTimeWarping.hpp"

using namespace std;

double dtwDist(class Curve *P, class Curve *Q)
{
    //dynamic time warping as instructed in the class
    //dynamic programming implementation
    
    int length = P->getSize();
    int width = Q->getSize();

    double **C = new double *[length];

    for (int i = 0; i < length; i++)
    {
        C[i] = new double[width];
    }

    double minimum, result = 0.0;

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                C[i][j] = euclideanDist(P->getCoord()[i], Q->getCoord()[j]);
            }
            else if (j > 0 && i == 0)
            {
                C[i][j] = C[i][j - 1] + euclideanDist(P->getCoord()[i], Q->getCoord()[j]);
            }
            else if (i > 0 && j == 0)
            {
                C[i][j] = C[i - 1][j] + euclideanDist(P->getCoord()[i], Q->getCoord()[j]);
            }
            else
            {
                minimum = min(min(C[i - 1][j], C[i - 1][j - 1]), C[i][j - 1]);
                C[i][j] = minimum + euclideanDist(P->getCoord()[i], Q->getCoord()[j]);
                minimum = 0.0;
            }
        }
    }

    result = C[length - 1][width - 1];

    for (int i = 0; i < length; i++)
    {
        delete[] C[i];
    }

    delete[] C;

    return result;
}