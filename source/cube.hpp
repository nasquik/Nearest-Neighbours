#ifndef CUBE_HPP
#define CUBE_HPP

#include "hashTable.hpp"
#include <vector>
using std::vector;
#include <map>
using std::map;

template <class T>
class Cube
{
    int k;
    int maxPoints;
    int probes;
    int w;
    vector<class Point *> *hyperCube;
    class HashTable<map<unsigned int, char> > *hashTables;

public:
    Cube(int, int, int, int, vector<class Point *> *);
    ~Cube();
    int getk();
    int getProbes();
    int getw();
    unsigned int getVertex(class Point *);
    int getMaxPoints();
    T findNN(class Point *, double *);
    vector<pair<class Point *, double> > *findRadiusNN(class Point *, double);
    int searchVertexRadiusNeighbors(vector<class Point *> *, class Point *, double, int *, vector<pair<class Point *, double> > *);
    int searchVertex(vector<class Point *> *, class Point *query, int *, double *, class Point **);
};

#endif