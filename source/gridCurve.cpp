#include <string>
#include <cmath>
#include <random>
#include <iostream>

#include "dataStructs.hpp"
#include "gridCurve.hpp"
#include "LSH.hpp"
#include "metrics.hpp"
#include "cube.hpp"
#include "bruteForce.hpp"

using namespace std;
////// GRID CURVES //////

template <class T>
gridCurve<T>::gridCurve(vector<class Curve *> *curves, int k, int L, int w, int probes, int minCurvePoints, int maxCurvePoints, double maxCoord)
{
    this->curves = curves;
    this->k = k;
    this->L = L;
    this->w = w;
    this->probes = probes;
    this->minCurvePoints = minCurvePoints;
    this->maxCurvePoints = maxCurvePoints;
    this->maxCoord = maxCoord * 1000;

    //use a multiplication of average distance of consecutive points for better results
    this->delta = 6 * calculateDelta();
    this->maxCurvePoints = maxCurvePoints * 2;

    this->displacement = new double *[L];

    //initialize the tau vectors
    for (int i = 0; i < L; i++)
    {
        this->displacement[i] = new double[2];
    }

    //create the tau vectors
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<> dis(0.0, this->delta);

    this->points = new vector<class Point *> *[L];

    //create the points produces from the snapped curves for each lsh structure

    for (int i = 0; i < L; i++)
    {
        points[i] = new vector<class Point *>;
        points[i]->reserve(curves->size());
    }

    for (int i = 0; i < this->L; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->displacement[i][j] = dis(generator);
        }

        for (int t = 0; t < curves->size(); t++)
        {
            points[i]->push_back(createVector(curves->at(t), i));
        }
    }
}

template <>
int gridCurve<class LSH<class Curve *> >::initializeAlgorithm()
{

    this->algorithm = new class LSH<class Curve *> *[L];

    //get an estimation of the average distance of a set of points to use as w
    for (int i = 0; i < L; i++)
    {
        this->w = 6 * calculateW(points[i], 2);
        this->algorithm[i] = new class LSH<class Curve *>(this->k, 1, this->w, points[i]);
    }

    return 0;
}

template <>
int gridCurve<class Cube<class Curve *> >::initializeAlgorithm()
{

    this->algorithm = new class Cube<class Curve *> *[L];
    //get an estimation of the average distance of a set of points to use as w
    for (int i = 0; i < L; i++)
    {
        this->w = 10 * calculateW(points[i], 2);
        this->algorithm[i] = new class Cube<class Curve *>(this->k, 100, this->probes, this->w, points[i]);
    }

    return 0;
}

template <class T>
int gridCurve<T>::initializeAlgorithm()
{
    return 0;
}

template <class T>
class Point *gridCurve<T>::gridCurve::createVector(class Curve *curve, int gridNo)
{

    //create the coord vector
    double *coord = new double[this->maxCurvePoints];

    //use the snapped value of the max coordinate as padding
    double snappedPadding1 = round((this->maxCoord - this->displacement[gridNo][0]) / this->delta);
    double snappedPadding2 = round((this->maxCoord - this->displacement[gridNo][1]) / this->delta);

    for (int i = 0; i < this->maxCurvePoints; i += 2)
    {
        coord[i] = snappedPadding1;
        coord[i + 1] = snappedPadding2;
    }

    double temp1 = 0, temp2 = 0;

    //snap the curves onto the grid
    int pos = 0;
    for (int i = 0; i < curve->getSize(); i++)
    {
        temp1 = (curve->getCoord()[i].first - this->displacement[gridNo][0]) / this->delta;
        temp2 = (curve->getCoord()[i].second - this->displacement[gridNo][1]) / this->delta;
        temp1 = round(temp1);
        temp2 = round(temp2);

        // remove duplicates
        if (i > 0)
        {
            if (coord[2 * pos] != temp1 || coord[(2 * pos) + 1] != temp2)
            {
                pos++;
                coord[2 * pos] = temp1;
                coord[(2 * pos) + 1] = temp2;
            }
        }
        else
        {
            coord[2 * i] = temp1;
            coord[(2 * i) + 1] = temp2;
        }
    }
    class Point *gridCurve = new class Point(curve->getID(), coord, this->maxCurvePoints, curve);

    return gridCurve;
}

template <>
class Curve *gridCurve<class LSH<class Curve *> >::findNN(class Curve *query, double *dist)
{
    class Point *queryGridCurve;
    class Curve *temp, *neighbor = NULL;
    double minDistance = numeric_limits<double>::max();

    //find the nearest curves among the LSH structures

    for (int i = 0; i < L; i++)
    {
        queryGridCurve = createVector(query, i);
        temp = algorithm[i]->findNN(queryGridCurve, dist);
        if (temp == NULL) //if no neighbor found
            continue;
        if (*dist < minDistance)
        {
            minDistance = *dist;
            neighbor = temp;
        }
    }

    if (neighbor == NULL)
        return NULL;

    return neighbor;
}

template <>
class Curve *gridCurve<class Cube<class Curve *> >::findNN(class Curve *query, double *dist)
{
    class Point *queryGridCurve;
    class Curve *temp, *neighbor = NULL;
    double minDistance = numeric_limits<double>::max();

    //find the nearest curves among the LSH structures

    for (int i = 0; i < L; i++)
    {
        queryGridCurve = createVector(query, i);
        temp = algorithm[i]->findNN(queryGridCurve, dist);
        if (temp == NULL) //if no neighbor found
            continue;
        if (*dist < minDistance)
        {
            minDistance = *dist;
            neighbor = temp;
        }
    }

    return neighbor;
}

template <class T>
class Curve *gridCurve<T>::findNN(class Curve *query, double *distance)
{
    return NULL;
}

template <class T>
gridCurve<T>::~gridCurve()
{
    //delete the structures and free the alocated memory
    for (int i = 0; i < L; i++)
    {
        delete displacement[i];
        delete algorithm[i];
        while (!points[i]->empty())
        {
            delete points[i]->back();
            points[i]->pop_back();
        }
        delete points[i];
    }

    delete[] displacement;
    delete[] points;
    delete[] algorithm;

}

template <class T>
double gridCurve<T>::calculateDelta()
{
    //calculate the average distance of consecutive points of the curves
    double curveAvg, totalAvg = 0, tempDist;
    int total = curves->size();

    for (int i = 0; i < curves->size(); i++)
    {
        curveAvg = 0;
        int size = curves->at(i)->getSize();
        if (size < 2)
        {
            total--;
            continue;
        }
        pair<double, double> *coords = curves->at(i)->getCoord();

        for (int j = 1; j < size; j++)
        {
            curveAvg += euclideanDist(coords[j], coords[j - 1]);
        }
        curveAvg /= (double)(size - 1);
        totalAvg += curveAvg;
    }
    totalAvg /= (double)total;

    return totalAvg;
}
template class gridCurve<class LSH<class Curve *> >;
template class gridCurve<class Cube<class Curve *> >;
