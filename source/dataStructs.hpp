#ifndef POINT_HPP
#define POINT_HPP

#include <string>
using std::string;
#include <utility>
using std::pair;
#include <vector>
using std::vector;

class Curve
{
    string ID;
    pair<double, double> *coordinates;
    int size;

public:
    Curve(){};
    ~Curve();
    Curve(string, pair<double, double> *, int);
    pair<double, double> *getCoord();
    int getSize();
    string getID();
};

class Point
{
    string ID;
    double *coordinates;
    int size;
    class Curve *curvePtr;

public:
    Point();
    Point(string ID, double *coord, int siz, class Curve *);
    ~Point();
    string getID();
    double *getCoord();
    int getSize();
    class Curve *getCurvePtr();
};

#endif