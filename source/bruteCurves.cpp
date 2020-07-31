#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <utility>

#include "dataStructs.hpp"
#include "fileReading.hpp"
#include "bruteForce.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    //calculates the nearest neighbors of the curves with brute force and dtw distance
    //and saves them into a file
    string inputFile, queryFile, outputFile;
    class Curve *q;
    clock_t timeBrute;
    pair<class Curve *, double> *bruteNN;
    ofstream brute;

    string optionBuffer, parameterBuffer;

    for (int i = 1; i <= (argc - 1); i += 2)
    {
        optionBuffer = string(argv[i]);
        parameterBuffer = string(argv[i + 1]);

        if (optionBuffer == "-d")
            inputFile = parameterBuffer;
        else if (optionBuffer == "-q")
            queryFile = parameterBuffer;
        else if (optionBuffer == "-o")
            outputFile = parameterBuffer;
    }

    class Reading reader;
    vector<class Curve *> *inputTable, *queryTable;
    int minpoints, maxpoints;
    double items;
    pair<vector<class Curve *> *, vector<class Curve *> *> input = reader.readCurves(inputFile, &minpoints, &maxpoints, &items);
    inputTable = input.first;
    queryTable = input.second;

    /////////calculate the nearest neighbors with brute force////////////////////
    brute.open(outputFile);
    brute.precision(15);

    for (int i = 0; i < queryTable->size(); i++)
    {
        q = (queryTable->at(i));
        timeBrute = clock();
        bruteNN = bruteForceCurve(inputTable, q);
        timeBrute = clock() - timeBrute;
        brute << bruteNN->first->getID() << " " << bruteNN->second << " " << timeBrute << endl;
        delete bruteNN;
    }

    brute.close();

    //delete the tables
    while (!inputTable->empty())
    {
        delete inputTable->back();
        inputTable->pop_back();
    }
    delete (inputTable);

    while (!queryTable->empty())
    {
        delete queryTable->back();
        queryTable->pop_back();
    }
    delete (queryTable);

    /////////////////////////////////////////////////////////////////////////////
}