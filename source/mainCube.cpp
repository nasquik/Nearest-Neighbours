#include <iostream>
#include <fstream>
#include <ctime>
#include <utility>
#include <sstream>

#include "dataStructs.hpp"
#include "fileReading.hpp"
#include "bruteForce.hpp"
#include "hashTable.hpp"
#include "cube.hpp"

using namespace std;

// $./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file>

int main(int argc, char *argv[])
{
    if (argc < 13)
    {
        cout << "Invalid Input!\n";
        return -1;
    }

    string inputFile, queryFile, outputFile;
    int k = 3, M = 10, probes = 2;

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
        else if (optionBuffer == "-M")
            M = stoi(parameterBuffer, nullptr, 10);
        else if (optionBuffer == "-probes")
            probes = stoi(parameterBuffer, nullptr, 10);
        else if (optionBuffer == "-o")
            outputFile = parameterBuffer;
    }

    int w = 4500;

    class Reading reader;
    int tableSize;
    vector<class Point *> *inputTable, *queryTable;
    pair<vector<class Point *> *, double> input = reader.readPoints(inputFile, 'i');
    inputTable = input.first;
    pair<vector<class Point *> *, double> queries = reader.readPoints(queryFile, 'q');
    queryTable = queries.first;
    double radius = queries.second;


    class Cube<class Point *> cubeImplementation(k, M, probes, w, inputTable);
    class Point *q, *b = NULL;
    double distance, tempAF, maxAF = 0.0, avgAF = 0.0;
    clock_t timeCube, timeBrute;

    pair<class Point *, double> *bruteNN;
    ofstream outfile;
    outfile.open(outputFile);

    ifstream readBrute("brutePoints.txt");

    string bruteNeighborID;
    string line;
    double bruteDist;

    for (int i = 0; i < queryTable->size(); i++)
    {
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

        q = queryTable->at(i);

        timeCube = clock();
        b = cubeImplementation.findNN(q, &distance);
        timeCube = clock() - timeCube;

        tempAF = distance / bruteDist;
        avgAF += tempAF;
        if (tempAF > maxAF)
            maxAF = tempAF;

        outfile << "Query Point: " << q->getID() << endl;
        if (b != NULL)
            outfile << "Nearest Neighbor Cube: " << b->getID() << endl
                    << "Distance Cube: " << distance << endl;
        else
            outfile << "Nearest Neighbor Cube: None Found!" << endl
                    << "Distance Cube: -" << endl;
        outfile << "True Neighbor: " << bruteNeighborID << endl
                << "DistanceTrue: " << bruteDist << endl;
        outfile << "tCube: " << (float)timeCube / CLOCKS_PER_SEC << endl
                << "tTrue: " << (float)timeBrute / CLOCKS_PER_SEC << endl;
        
        //select which method of NN to run according to the radius
        if (radius > 0.0)
        {
            vector<pair<class Point *, double> > *radiusNeighbors = cubeImplementation.findRadiusNN(q, radius);

            outfile << "R-nearest neighbors:" << endl;

            if (radiusNeighbors != NULL)
            {
                for (int i = 0; i < radiusNeighbors->size(); i++)
                {
                    outfile << "ID: " << radiusNeighbors->at(i).first->getID() << " Distance: " << radiusNeighbors->at(i).second << endl;
                }
                outfile << endl;

                radiusNeighbors->clear();
                delete (radiusNeighbors);
            }
            else
                outfile << "None Found!" << endl
                        << endl;
        }
    }

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