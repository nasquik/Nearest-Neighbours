#ifndef _FILE_READING
#define _FILE_READING

#include <vector>
#include <string>

using std::string;
using std::vector;

#include <utility>
using std::pair;

class Reading

{
public:
    pair<vector<class Point *> *, double> readPoints(string, char);
    pair<vector<class Curve *> *, vector<class Curve *> *> readCurves(string, int *, int *, double *);
};

#endif