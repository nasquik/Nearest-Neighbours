#include <iostream>
#include <limits>
#include <cstring>
#include <cmath>

#include "dataStructs.hpp"
#include "metrics.hpp"
#include "dynamicTimeWarping.hpp"
#include "hashTable.hpp"
#include "LSH.hpp"

using namespace std;

// Class used for the implementation of the LSH algorithm
template <class T>
LSH<T>::LSH(int k, int L, int w, vector<class Point *> *input)
{
    this->k = k;
    this->L = L;
    this->w = w;

    this->input = input;

    int dim = input->at(0)->getSize();

    unsigned int hashtableSize = input->size() / 8;
    hashTables = new class HashTable<vector<pair<class Point *, unsigned int> > >[L];
    for (int i = 0; i < L; i++)
    {
        hashTables[i].initialize(hashtableSize, w, k, dim, 0);
    }
    //insert the points to the hashtables
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < input->size(); j++)
        {
            hashTables[i].insertPoint(input->at(j));
        }
    }
}

template <class T>
LSH<T>::~LSH()
{
    delete[] hashTables;
}

template <class T>
int LSH<T>::getk()
{
    return k;
}

template <class T>
int LSH<T>::getL()
{
    return L;
}

template <class T>
int LSH<T>::getw()
{
    return w;
}

template <>
class Point *LSH<class Point *>::findNN(class Point *query, double *dist)
{

    //find the nearest neighbors
    class Point *b = NULL;
    class Point *p = NULL;
    double manhattanD;
    double distance = numeric_limits<double>::max();
    vector<pair<class Point *, unsigned int> > neighbors;
    int count;

    for (int i = 0; i < L; i++)
    {
        unsigned int amplifiedResult = 0;
        neighbors = hashTables[i].getneighbors(query, &amplifiedResult);
        count = 0;

        for (int j = 0; j < neighbors.size(); j++)
        {
            if (amplifiedResult == neighbors.at(j).second) //compare two points only if they have the same hashfunction value
            {
                count++;
                if (count > 10 * L)
                    break;

                p = neighbors.at(j).first;
                manhattanD = manhattanDist(query, p);

                if (manhattanD < distance)
                {
                    b = p;
                    distance = manhattanD;
                }
            }
        }
    }
    *dist = distance;

    return b;
}

template <>
class Curve *LSH<class Curve *>::findNN(class Point *query, double *dist)
{
    class Point *b = NULL;
    class Point *p = NULL;
    double dtwD;
    double distance = numeric_limits<double>::max();
    vector<pair<class Point *, unsigned int> > neighbors;
    int count;

    for (int i = 0; i < L; i++)
    {
        unsigned int amplifiedResult = 0;
        neighbors = hashTables[i].getneighbors(query, &amplifiedResult);
        count = 0;

        for (int j = 0; j < neighbors.size(); j++)
        {
            if (amplifiedResult == neighbors.at(j).second)
            {
                count++;
                if (count > 30 * L)
                    break;

                p = neighbors.at(j).first;
                dtwD = dtwDist(query->getCurvePtr(), p->getCurvePtr());

                if (dtwD < distance)
                {
                    b = p;
                    distance = dtwD;
                }
            }
        }
    }

    *dist = distance;
    if (b != NULL)
        return b->getCurvePtr();
    else
        return NULL;
}

template <class T>
T LSH<T>::findNN(class Point *query, double *dist) {}

template <>
vector<pair<class Point *, double> > *LSH<class Point *>::findRadiusNN(class Point *query, double radius)
{

    class Point *p = NULL;
    double manhattanD;
    vector<pair<class Point *, double> > *radiusNaighbors = new vector<pair<class Point *, double> >;
    vector<pair<class Point *, unsigned int> > neighbors;
    int count;

    for (int i = 0; i < L; i++)
    {
        unsigned int amplifiedResult = 0;
        neighbors = hashTables[i].getneighbors(query, &amplifiedResult);
        count = 0;

        for (int j = 0; j < neighbors.size(); j++)
        {
            if (amplifiedResult == neighbors.at(j).second)
            {
                count++;
                if (count > 10 * L)
                    break;

                p = neighbors.at(j).first;
                manhattanD = manhattanDist(query, p);

                if (manhattanD <= radius)
                {
                    pair<class Point *, double> neighborRadius;
                    neighborRadius.first = p;
                    neighborRadius.second = manhattanD;
                    radiusNaighbors->push_back(neighborRadius);
                }
            }
        }
    }

    if (!radiusNaighbors->empty())
        return radiusNaighbors;
    else
    {
        delete radiusNaighbors;
        return NULL;
    }
}

template <class T>
vector<pair<class Point *, double> > *LSH<T>::findRadiusNN(class Point *query, double radius)
{
    return NULL;
}

template class LSH<class Curve *>;
template class LSH<class Point *>;