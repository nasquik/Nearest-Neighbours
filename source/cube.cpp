#include <iostream>
#include <limits>
#include <bitset>

#include "dataStructs.hpp"
#include "metrics.hpp"
#include "dynamicTimeWarping.hpp"
#include "hashTable.hpp"
#include "cube.hpp"

using namespace std;

// Class used for the implementation of the randomized projections on a hypercube algorithm

template <class T>
Cube<T>::Cube(int givenk, int givenMaxPoints, int givenProbes, int givenw, vector<class Point *> *points)
{
    k = givenk; //number of dimensions
    maxPoints = givenMaxPoints;
    probes = givenProbes;
    w = givenw;

    hyperCube = points;
    int dim = hyperCube->at(0)->getSize();

    unsigned int hashtableSize = hyperCube->size() / 8;
    
    //create the unordered map for the cube
    hashTables = new class HashTable<map<unsigned int, char> >[k];

    for (int i = 0; i < k; i++)
    {
        hashTables[i].initialize(hashtableSize, w, k, dim, givenProbes);
    }

    //create a list of vectors where the points will be saved
    //calculate the size of the points
    unsigned int temp = 1;
    temp = temp << k;
    hyperCube = new vector<class Point *>[temp];

    unsigned int vertex;
    //insert the points into the hypercube
    for (int i = 0; i < points->size(); i++)
    {
        vertex = getVertex(points->at(i));
        hyperCube[vertex].push_back(points->at(i));
    }
}

template <class T>
unsigned int Cube<T>::getVertex(class Point *point)
{
    //get the bitstring that represents the acme of the given class Point
    unsigned int vertex = 0;
    unsigned int hashresult;
    for (int j = 0; j < k; j++)
    {
        hashresult = hashTables[j].insertPoint(point);
        hashresult = hashresult << j;
        vertex += hashresult;
    }
    return vertex;
}

template <class T>
Cube<T>::~Cube()
{
    delete[] hashTables;
    delete[] hyperCube;
}

template <class T>
int Cube<T>::getk()
{
    return k;
}

template <class T>
int Cube<T>::getProbes()
{
    return probes;
}

template <class T>
int Cube<T>::getw()
{
    return w;
}

template <class T>
int Cube<T>::getMaxPoints()
{
    return maxPoints;
}

template <>
int Cube<class Curve *>::searchVertex(vector<class Point *> *vertex, class Point *query, int *count, double *distance, class Point **b)
{

    double DTWDist;
    class Point *p;
    //get the closest curves according to their dtw distance
    for (int j = 0; j < vertex->size(); j++)
    {
        *count = *count + 1;
        if (*count > maxPoints)
            return 1;

        p = vertex->at(j);
        DTWDist = dtwDist(query->getCurvePtr(), p->getCurvePtr());

        if (DTWDist < *distance)
        {
            *b = p;
            *distance = DTWDist;
        }
    }
    return 0;
}

template <class T>
int Cube<T>::searchVertex(vector<class Point *> *vertex, class Point *query, int *count, double *distance, class Point **b)
{

    double manhattanD;
    class Point *p;
    for (int j = 0; j < vertex->size(); j++)
    {
        *count = *count + 1;
        if (*count > maxPoints)
            return 1;

        p = vertex->at(j);
        manhattanD = manhattanDist(query, p);

        if (manhattanD < *distance)
        {
            *b = p;
            *distance = manhattanD;
        }
    }
    return 0;
}

template <>
class Point *Cube<class Point *>::findNN(class Point *query, double *dist)
{
    class Point *p, *b = NULL;
    double distance = numeric_limits<double>::max();
    int count;
    int probeCounter = 0;
    unsigned int vertexNo = getVertex(query);
    vector<class Point *> *vertex = NULL;
    unsigned int mask = 1, pos;
    count = 0;
    vector<pair<unsigned int, unsigned int> > *neighborPos1, *neighborPos2;
    neighborPos1 = new vector<pair<unsigned int, unsigned int> >;

    probeCounter = 0;

    //first for the nearest neighbor inside the given vertex and its neighbors
    //that have a hamming distance of 1 
    for (int i = 0; i < k; i++)
    {

        //check for probes
        if (probeCounter == probes)
        {
            neighborPos1->clear();
            delete neighborPos1;
            *dist = distance;
            return b;
        }

        if (i == 0)
        {
            vertex = &hyperCube[vertexNo];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertex(vertex, query, &count, &distance, &b))
            {
                neighborPos1->clear();
                delete neighborPos1;
                *dist = distance;
                return b;
            }
        }
        else
        {
            if (i > 1)
                mask = mask << 1;
            pos = vertexNo ^ mask;
            vertex = &hyperCube[pos];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertex(vertex, query, &count, &distance, &b))
            {
                neighborPos1->clear();
                delete neighborPos1;
                *dist = distance;
                return b;
            }
            neighborPos1->push_back(make_pair(pos, i));
        }
    }
    int canProduce;
    int newPos;

    //produce the vertexes that have a hamming distance bigger than 1 
    for (int i = 0; i < k - 1; i++)
    {
        neighborPos2 = new vector<pair<unsigned int, unsigned int> >;
        for (int j = 0; j < neighborPos1->size(); j++)
        {
            if (probeCounter == probes)
            {
                neighborPos1->clear();
                delete neighborPos1;
                neighborPos2->clear();
                delete neighborPos2;
                *dist = distance;
                return b;
            }
            //try to produce the neighbors with hamming distance bigger by one
            canProduce = neighborPos1->at(j).second;
            mask = 1;
            mask = mask << (canProduce - 1);
            for (int k = 0; k < canProduce; k++)
            {
                pos = neighborPos1->at(j).first ^ mask;
                newPos = canProduce - k - 1;
                if (searchVertex(vertex, query, &count, &distance, &b))
                {
                    neighborPos1->clear();
                    delete neighborPos1;
                    neighborPos2->clear();
                    delete neighborPos2;
                    *dist = distance;
                    return b;
                }
                mask = mask >> 1;
                if (vertex->size() == 0)
                {
                    continue;
                }
                probeCounter++;
                neighborPos2->push_back(make_pair(pos, newPos));
            }
        }
        // empty the first vector and fill it with the second
        neighborPos1->clear();
        delete neighborPos1;
        neighborPos1 = neighborPos2;
    }

    *dist = distance;
    return b;
}

template <>
class Curve *Cube<class Curve *>::findNN(class Point *query, double *dist)
{
    class Point *p, *b = NULL;
    double distance = numeric_limits<double>::max();
    int count;
    int probeCounter = 0;
    unsigned int vertexNo = getVertex(query);
    vector<class Point *> *vertex = NULL;
    unsigned int mask = 1, pos;
    count = 0;
    vector<pair<unsigned int, unsigned int> > *neighborPos1, *neighborPos2;
    neighborPos1 = new vector<pair<unsigned int, unsigned int> >;

    probeCounter = 0;

    for (int i = 0; i < k; i++)
    {
        //check for probes
        if (probeCounter == probes)
        {
            neighborPos1->clear();
            delete neighborPos1;
            *dist = distance;
            return b->getCurvePtr();
        }

        if (i == 0)
        {
            vertex = &hyperCube[vertexNo];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertex(vertex, query, &count, &distance, &b))
            {
                neighborPos1->clear();
                delete neighborPos1;
                *dist = distance;
                return b->getCurvePtr();
            }
        }
        else
        {
            if (i > 1)
                mask = mask << 1;
            pos = vertexNo ^ mask;
            vertex = &hyperCube[pos];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertex(vertex, query, &count, &distance, &b))
            {
                neighborPos1->clear();
                delete neighborPos1;
                *dist = distance;
                return b->getCurvePtr();
            }
            neighborPos1->push_back(make_pair(pos, i));
        }
    }

    int canProduce;
    int newPos;

    for (int i = 0; i < k - 1; i++)
    {
        neighborPos2 = new vector<pair<unsigned int, unsigned int> >;
        for (int j = 0; j < neighborPos1->size(); j++)
        {
            if (probeCounter == probes)
            {
                neighborPos1->clear();
                delete neighborPos1;
                neighborPos2->clear();
                delete neighborPos2;
                *dist = distance;
                return b->getCurvePtr();
            }
            //try to produce the neighbors with hamming distance bigger by one
            canProduce = neighborPos1->at(j).second;
            mask = 1;
            mask = mask << (canProduce - 1);

            for (int k = 0; k < canProduce; k++)
            {
                pos = neighborPos1->at(j).first ^ mask;
                newPos = canProduce - k - 1;
                if (searchVertex(vertex, query, &count, &distance, &b))
                {
                    neighborPos1->clear();
                    delete neighborPos1;
                    neighborPos2->clear();
                    delete neighborPos2;
                    *dist = distance;
                    return b->getCurvePtr();
                }
                mask = mask >> 1;
                if (vertex->size() == 0)
                {
                    continue;
                }
                probeCounter++;
                neighborPos2->push_back(make_pair(pos, newPos));
            }
        }
        // empty the first vector and fill it with the second
        neighborPos1->clear();
        delete neighborPos1;
        neighborPos1 = neighborPos2;
    }

    *dist = distance;
    return b->getCurvePtr();
}

template <class T>
T Cube<T>::findNN(class Point *query, double *dist)
{
    return NULL;
}

template <>
int Cube<class Point *>::searchVertexRadiusNeighbors(vector<class Point *> *vertex, class Point *query, double radius, int *count, vector<pair<class Point *, double> > *neighbors)
{
    //same operation but with the radius parameter
    double manhattanD;
    class Point *p;

    for (int j = 0; j < vertex->size(); j++)
    {
        *count = *count + 1;

        if (*count > maxPoints)
            return 1;

        p = vertex->at(j);
        manhattanD = manhattanDist(query, p);

        if (manhattanD <= radius)
        {
            pair<class Point *, double> neighbor;
            neighbor.first = p;
            neighbor.second = manhattanD;
            neighbors->push_back(neighbor);
        }
    }

    return 0;
}

template <class T>
int Cube<T>::searchVertexRadiusNeighbors(vector<class Point *> *vertex, class Point *query, double radius, int *count, vector<pair<class Point *, double> > *neighbors)
{
    return 0;
}

template <>
vector<pair<class Point *, double> > *Cube<class Point *>::findRadiusNN(class Point *query, double radius)
{
    //same operation but with the radius parameter
    vector<pair<class Point *, double> > *radiusNeighbors = new vector<pair<class Point *, double> >;

    unsigned int vertexNo = getVertex(query);
    vector<class Point *> *vertex;

    int count = 0;
    vector<pair<unsigned int, unsigned int> > *neighborPos1, *neighborPos2;
    neighborPos1 = new vector<pair<unsigned int, unsigned int> >;

    unsigned int mask = 1, pos;

    int probeCounter = 0;

    for (int i = 0; i < k; i++)
    {

        //check for probes
        if (probeCounter == probes)
        {
            neighborPos1->clear();
            delete neighborPos1;
            if (!radiusNeighbors->empty())
                return radiusNeighbors;
            else
            {
                delete radiusNeighbors;
                return NULL;
            }
        }

        if (i == 0)
        {
            vertex = &hyperCube[vertexNo];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertexRadiusNeighbors(vertex, query, radius, &count, radiusNeighbors))
            {
                neighborPos1->clear();
                delete neighborPos1;
                if (!radiusNeighbors->empty())
                    return radiusNeighbors;
                else
                {
                    delete radiusNeighbors;
                    return NULL;
                }
            }
        }
        else
        {
            if (i > 1)
                mask = mask << 1;
            pos = vertexNo ^ mask;
            vertex = &hyperCube[pos];
            if (vertex->size() == 0)
            {
                continue;
            }
            probeCounter++;
            if (searchVertexRadiusNeighbors(vertex, query, radius, &count, radiusNeighbors))
            {
                neighborPos1->clear();
                delete neighborPos1;
                if (!radiusNeighbors->empty())
                    return radiusNeighbors;
                else
                {
                    delete radiusNeighbors;
                    return NULL;
                }
            }
            neighborPos1->push_back(make_pair(pos, i));
        }
    }
    int canProduce;
    int newPos;

    for (int i = 0; i < k - 1; i++)
    {
        neighborPos2 = new vector<pair<unsigned int, unsigned int> >;
        for (int j = 0; j < neighborPos1->size(); j++)
        {
            if (probeCounter == probes)
            {
                neighborPos1->clear();
                delete neighborPos1;
                neighborPos2->clear();
                delete neighborPos2;
                if (!radiusNeighbors->empty())
                    return radiusNeighbors;
                else
                {
                    delete radiusNeighbors;
                    return NULL;
                }
            }
            //try to produce the neighbors with hamming distance bigger by one
            canProduce = neighborPos1->at(j).second;
            mask = 1;
            mask = mask << (canProduce - 1);
            for (int k = 0; k < canProduce; k++)
            {
                pos = neighborPos1->at(j).first ^ mask;
                newPos = canProduce - k - 1;
                if (searchVertexRadiusNeighbors(vertex, query, radius, &count, radiusNeighbors))
                {
                    neighborPos1->clear();
                    delete neighborPos1;
                    neighborPos2->clear();
                    delete neighborPos2;
                    if (!radiusNeighbors->empty())
                        return radiusNeighbors;
                    else
                    {
                        delete radiusNeighbors;
                        return NULL;
                    }
                }
                mask = mask >> 1;
                if (vertex->size() == 0)
                {
                    continue;
                }
                probeCounter++;
                neighborPos2->push_back(make_pair(pos, newPos));
            }
        }
        // empty the first vector and fill it with the second
        neighborPos1->clear();
        delete neighborPos1;
        neighborPos1 = neighborPos2;
    }

    if (!radiusNeighbors->empty())
        return radiusNeighbors;
    else
    {
        delete radiusNeighbors;
        return NULL;
    }
}

template <class T>
vector<pair<class Point *, double> > *Cube<T>::findRadiusNN(class Point *query, double radius)
{
    return NULL;
}

template class Cube<class Point *>;
template class Cube<class Curve *>;