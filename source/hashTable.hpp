#ifndef _HASHTABLE_
#define _HASHTABLE_

#include <vector>
using std::vector;
#include <utility>
using std::pair;
#include <random>
using std::mt19937;
using std::random_device;
#include <cstring>

template <class T>
class HashTable
{
    unsigned int bucketSize;
    int numFunct;
    int w;
    int dimensions;
    int maxPoints;
    mt19937 generator;
    unsigned int M;
    T *buckets;
    double **sVectors;
    unsigned int *mArray;

public:
    HashTable();
    void initialize(size_t, int, int, int, int);
    void printHashTable();
    ~HashTable();

    //hashfunction operations
    unsigned int amplifiedHashFunctionPoint(class Point *x);
    unsigned int hashFunctionPoint(class Point *x, int functionNo);
    int insertPoint(class Point *);
    int findPoint(class Point *);
    T &getneighbors(class Point *x, unsigned int *amplifiedResult)
    {
        *amplifiedResult = amplifiedHashFunctionPoint(x);
        return buckets[*amplifiedResult % bucketSize];
    }
    T getDigit(class Point *x, unsigned int *amplifiedResult)
    {
        *amplifiedResult = amplifiedHashFunctionPoint(x);
        return buckets[*amplifiedResult % bucketSize];
    }
};

unsigned int modular_expo(unsigned int base, unsigned int exponent, unsigned int modulus);

#endif