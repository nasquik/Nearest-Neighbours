#include <iostream>
#include <fstream>
#include <ctime>
#include <utility>
#include <sstream>

#include "dataStructs.hpp"
#include "fileReading.hpp"
#include "bruteForce.hpp"
#include "LSH.hpp"

using namespace std;

// $./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>

int main(int argc, char *argv[])
{
    if (argc > 11)
    {
        cout << "Invalid Input!\n";
        return -1;
    }

    string inputFile, queryFile, outputFile;
    int k = 3, L = 5;

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

    int w = 4500;

    class Reading reader;
    int tableSize;
    vector<class Point *> *inputTable, *queryTable;
    pair<vector<class Point *> *, double> input = reader.readPoints(inputFile, 'i');
    inputTable = input.first;
    pair<vector<class Point *> *, double> queries = reader.readPoints(queryFile, 'q');
    queryTable = queries.first;
    double radius = queries.second;

    class LSH<class Point *> lshImplementation(k, L, w, inputTable);
    class Point *q, *b = NULL;
    double distance, tempAF, maxAF = 0.0, avgAF = 0.0;
    clock_t timeLSH, timeBrute;

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

        timeLSH = clock();
        b = lshImplementation.findNN(q, &distance);
        timeLSH = clock() - timeLSH;

        tempAF = distance / bruteDist;
        avgAF += tempAF;
        if (tempAF > maxAF)
            maxAF = tempAF;

        outfile << "Query Point: " << q->getID() << endl;
        if (b != NULL)
            outfile << "Nearest Neighbor LSH: " << b->getID() << endl
                    << "Distance LSH: " << distance << endl;
        else
            outfile << "Nearest Neighbor LSH: None Found!" << endl
                    << "Distance LSH: -" << endl;
        outfile << "True Neighbor: " << bruteNeighborID << endl
                << "DistanceTrue: " << bruteDist << endl;
        outfile << "tLSH: " << (float)timeLSH / CLOCKS_PER_SEC << endl
                << "tTrue: " << (float)timeBrute / CLOCKS_PER_SEC << endl;

        if (radius > 0.0)
        {
            vector<pair<class Point *, double> > *radiusNeighbors = lshImplementation.findRadiusNN(q, radius);

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
