#ifndef LSH_HPP
#define LSH_HPP

#include <vector>
using std::vector;

#include "hashTable.hpp"

template <class T>
class LSH
{
    int k;
    int w;
    int L;
    vector<class Point *> *input;
    class HashTable<vector<pair<class Point *, unsigned int> > > *hashTables;

public:
    LSH(int, int, int, vector<class Point *> *);
    ~LSH();
    int getk();
    int getw();
    int getL();
    T findNN(class Point *, double *);
    vector<pair<class Point *, double> > *findRadiusNN(class Point *, double);
};

#endif