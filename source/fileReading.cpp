#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

#include "dataStructs.hpp"
#include "fileReading.hpp"

using namespace std;

//Class used to read the input files

pair<vector<class Point *> *, double> Reading::readPoints(string FileLocation, char identifier)
{
    ifstream infile(FileLocation);
    string line;
    //count the lines of the file
    int count = 0;
    int columns = 0;
    int x;
    ifstream file(FileLocation);

    if (identifier == 'q')
        getline(file, line);

    if (getline(file, line))
    {
        count++;
        //count the columns
        istringstream buffer(line);
        while (buffer >> x)
        {
            columns++;
        }
        columns--; //exclude the Point name
    }
    while (getline(file, line))
        count++;

    cout << "Number of lines in file: " << count << endl;
    cout << "Number of dimensions per point: " << columns << endl;

    //construct the table of Points
    vector<class Point *> *table = new vector<class Point *>;
    table->reserve(count);

    int j = 0;
    string name;
    string buff;
    double radius = -1.0;
    double *coord;

    if (identifier == 'q')
    {
        getline(infile, line);
        istringstream iss(line);

        iss >> name;

        if (name == "Radius:")
        {
            iss >> radius;
        }
    }

    while (getline(infile, line))
    {
        istringstream iss(line);

        //get the name
        iss >> name;

        coord = new double[columns];

        for (int i = 0; i < columns; i++)
        {
            iss >> coord[i];
        }
        class Point *newPoint = new class Point(name, coord, columns, NULL);
        table->push_back(newPoint);
        j++;
    }

    pair<vector<class Point *> *, double> PAIR;
    PAIR.first = table;
    PAIR.second = radius;

    return PAIR;
}

pair<vector<class Curve *> *, vector<class Curve *> *> Reading::readCurves(string FileLocation, int *minPoints, int *maxPoints, double *maxCoord)
{
    ifstream infile(FileLocation);
    string line;
    vector<class Curve *> *inputTable = new vector<class Curve *>;
    vector<class Curve *> *queryTable = new vector<class Curve *>;

    pair<vector<class Curve *> *, vector<class Curve *> *> input;
    input.first = inputTable;
    input.second = queryTable;

    int minpoints = __INT_MAX__;
    int maxpoints = 0;
    int j = 0;
    while (getline(infile, line))
    {
        istringstream iss(line);
        int points;
        string ID, buff;
        iss >> ID;
        iss >> points;
        pair<double, double> *coord = new pair<double, double>[points];
        pair<double, double> mypair;
        for (size_t i = 0; i < points; i++)
        {
            //read first part of the string
            iss >> buff;
            sscanf(buff.c_str(), "(%lf,", &coord[i].first);
            iss >> buff;
            sscanf(buff.c_str(), "%lf)", &coord[i].second);

            if (coord[i].first > *maxCoord)
            {
                *maxCoord = coord[i].first;
            }

            if (coord[i].second > *maxCoord)
            {
                *maxCoord = coord[i].second;
            }
        }
        class Curve *curve = new class Curve(ID, coord, points);
        j++;
        //read the last 87 points as queries
        if (j > 7400)
        {
            input.second->push_back(curve); //enter the queries
        }
        else
        {
            input.first->push_back(curve); //enter the input curves
        }

        if (minpoints > points)
        {
            minpoints = points;
        }
        if (maxpoints < points)
        {
            maxpoints = points;
        }
    }

    *minPoints = minpoints;
    *maxPoints = maxpoints;
    return input;
}