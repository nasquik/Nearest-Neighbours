#ifndef GRIDCURVE_HPP
#define GRIDCURVE_HPP

#include <string>
using std::string;
#include <vector>
using std::vector;

template <class T>
class gridCurve
{
    vector<class Curve *> *curves;
    vector<class Point *> **points;
    int k;
    int L;
    int w;
    int probes;
    int maxCurvePoints;
    int minCurvePoints;
    double delta;
    double maxCoord;
    double **displacement;
    T **algorithm;

public:
    gridCurve(){};
    ~gridCurve();
    gridCurve(vector<class Curve *> *, int, int, int, int, int, int, double);
    int initializeAlgorithm();
    class Point *createVector(class Curve *, int);
    class Curve *findNN(class Curve *, double *);
    double calculateDelta();
};

#endif
