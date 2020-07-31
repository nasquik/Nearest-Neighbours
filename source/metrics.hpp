#ifndef METRICS_HPP
#define METRICS_HPP

#include <utility>
using std::pair;

double manhattanDist(class Point *, class Point *);
double manhattanDist(pair<double, double>, pair<double, double>);
double euclideanDist(pair<double, double>, pair<double, double>);

#endif