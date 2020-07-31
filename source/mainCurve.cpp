#include <iostream>
#include <fstream>
#include <ctime>
#include <utility>
#include <sstream>

#include "dataStructs.hpp"
#include "fileReading.hpp"
#include "bruteForce.hpp"
#include "gridCurve.hpp"
#include "LSH.hpp"
#include "cube.hpp"

using namespace std;

// $./curve –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -a
// {LSH, RandomProjection} –h {LSH, Hypercube} -e <double>

int main(int argc, char *argv[])
{

    ////// Read command line arguments //////
    if (argc > 15)
    {
        cout << "Invalid Input!\n";
        return -1;
    }

    string inputFile, queryFile, outputFile;
    string choice1, choice2;
    int k = 3, L = 5;
    double e = 0.5;

    string optionBuffer, parameterBuffer;

    for (int i = 1; i <= (argc - 1); i += 2)
    {
        optionBuffer = string(argv[i]);
        parameterBuffer = string(argv[i + 1]);

        if (optionBuffer == "-d")
            inputFile = parameterBuffer;
        else if (optionBuffer == "-q")
            queryFile = parameterBuffer;
        else if (optionBuffer == "-k")
            k = stoi(parameterBuffer, nullptr, 10);
        else if (optionBuffer == "-L")
            L = stoi(parameterBuffer, nullptr, 10);
        else if (optionBuffer == "-o")
            outputFile = parameterBuffer;
    }

    ////// Initialize input and queries //////

    int w = 140;

    class Reading reader;
    int tableSize;
    int minPoints = 0, maxPoints = 0;

    vector<class Curve *> *inputTable, *queryTable;
    pair<vector<class Curve *> *, vector<class Curve *> *> tables;

    double maxCoord = -10000000;

    tables = reader.readCurves(inputFile, &minPoints, &maxPoints, &maxCoord);
    inputTable = tables.first;
    queryTable = tables.second;

    ofstream outfile;

    outfile.open(outputFile);
    outfile.precision(16);
    double tempAF, maxAF = 0.0, avgAF = 0.0;

    class Curve *q, *b = NULL;
    double distance;
    maxAF = 0.0;
    avgAF = 0.0;
    clock_t timeAlgorithm, timeBrute;

    pair<class Curve *, double> *bruteNN;


    ifstream readBrute("bruteCurves.txt");
    string line, bruteNeighborID;
    double bruteDist;

    ////// Run algorithm based on the user's choices
    class gridCurve<class LSH<class Curve *> > *lshLsh = new class gridCurve<class LSH<class Curve *> >(inputTable, k, L, w, 0, minPoints, maxPoints, maxCoord);
    lshLsh->initializeAlgorithm();

    for (int i = 0; i < queryTable->size(); i++)
    {
        q = (queryTable->at(i));
        timeAlgorithm = clock();
        b = lshLsh->findNN(q, &distance);
        timeAlgorithm = clock() - timeAlgorithm;

        //read the output of the bruteforce file
        if (getline(readBrute, line))
        {
            istringstream buffer(line);
            buffer >> bruteNeighborID >> bruteDist >> timeBrute;
        }
        else
        {
            cout << "ERROR reading bruteforce file!" << endl;
        }
        ////////////////////////////////

        tempAF = distance / bruteDist;
        if (distance == 0 || bruteDist == 0)
            tempAF = 1;
        cout.precision(15);
        cout << "Curve ID: " << q->getID() << " Current tempAF: " << tempAF << endl;
        avgAF += tempAF;
        if (tempAF > maxAF)
            maxAF = tempAF;

        
        //print the output results to the outfile
        outfile << "Query Point: " << q->getID() << endl;
        if (b != NULL)
            outfile << "Nearest Neighbor LSH/LSH: " << b->getID() << endl
                    << "Distance LSH/LSH: " << distance << endl;
        else
            outfile << "Nearest Neighbor LSH/LSH: None Found!" << endl
                    << "Distance LSH/LSH: -" << endl;
        outfile << "True Neighbor: " << bruteNeighborID << endl
                << "DistanceTrue: " << bruteDist << endl;
        outfile << "tLSH/LSH: " << (float)timeAlgorithm / CLOCKS_PER_SEC << endl
                << "tTrue: " << (float)timeBrute / CLOCKS_PER_SEC << endl
                << endl;
    }
    delete lshLsh;

    cout << "MaxAF: " << maxAF << endl;
    cout << "AvgAF: " << avgAF / queryTable->size() << endl;
    outfile.close();

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
}
