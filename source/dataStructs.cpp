#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <random>

#include "dataStructs.hpp"
#include "LSH.hpp"

using namespace std;

/////// POINTS //////

Point::Point(string ID, double *coord, int siz, class Curve *ptr)
{
    this->ID = ID;
    coordinates = coord;
    size = siz;
    curvePtr = ptr;
}

Point::~Point()
{
    delete[] coordinates;
}

string Point::getID()
{
    return ID;
}

int Point::getSize()
{
    return size;
}

double *Point::getCoord()
{
    return coordinates;
}

class Curve *Point::getCurvePtr()
{
    return curvePtr;
}

////// CURVES //////

Curve::Curve(string givenID, pair<double, double> *coords, int size)
{
    ID = givenID;
    coordinates = coords;
    this->size = size;
}

pair<double, double> *Curve::getCoord()
{
    return coordinates;
}

int Curve::getSize()
{
    return size;
}

Curve::~Curve()
{
    delete coordinates;
}

string Curve::getID()
{
    return ID;
}