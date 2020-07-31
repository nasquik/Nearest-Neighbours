#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

void printAllPathsUtil(int **array,vector<vector <pair<int,int> >* > *relevantTraversals, int i, int j, int m, int n, pair<int,int> *path, int pi) 
{ 
    // Reached the bottom of the matrix so we are left with 
    // only option to move right 
    if (i == m - 1) 
    { 
        for (int k = j; k < n; k++) 
            path[pi + k - j] = make_pair(i, k); //pair i,k
        
        vector <pair<int,int> >* newVec = new vector <pair<int,int> >;
        newVec->reserve(pi + n - j);
	for (int l = 0; l < pi + n - j; l++){
            //cout <<"("<< path[l].first << " " << path[l].second<< ") "; 
            newVec->push_back(path[l]);
        } 
        relevantTraversals->push_back(newVec);

        //cout << endl; 
        return; 
    } 
  
    // Reached the right corner of the matrix we are left with 
    // only the downward movement. 
    if (j == n - 1) 
    { 
        for (int k = i; k < m; k++) 
            path[pi + k - i] = make_pair(k, j); //pair k,j

        vector <pair<int,int> >* newVec = new vector <pair<int,int> >;
        newVec->reserve(pi+m-i);
	    for (int l = 0; l < pi + m - i; l++)
        {
            //cout <<"("<< path[l].first << " " << path[l].second<< ") "; 
            newVec->push_back(path[l]);
        } 
        relevantTraversals->push_back(newVec);
        cout << endl; 
        return; 
    } 
  
    // Add the current cell to the path being generated 
    path[pi] = make_pair(i, j);  //pair i,j
  
    // Print all the paths that are possible after moving down
    if ((i + 1 < m) && (array[i+1][j] == 1)) 
        printAllPathsUtil(array, relevantTraversals,i + 1, j, m, n, path, pi + 1); 
  
    // Print all the paths that are possible after moving right 
    if((j + 1 < n) && (array[i][j+1] == 1))
        printAllPathsUtil(array, relevantTraversals, i, j + 1, m, n, path, pi + 1); 
  

    // Print all the paths that are possible after moving diagonal 
    if ((i + 1 < m) && (j + 1 < n) && (array[i+1][j+1] == 1))
        printAllPathsUtil(array , relevantTraversals,i + 1, j + 1, m, n, path, pi + 1); 
} 
  
// The main function that prints all paths from top left to bottom right 
// in a matrix 'mat' of size mXn 
vector<vector <pair<int,int> >* >* printAllPaths(int**array, int m, int n) 
{
    pair<int,int> *path = new pair<int,int>[m + n];
    cout << "hi " << m << " " << n << endl;
    vector<vector <pair<int,int> >* > *relevantTraversals = new vector<vector <pair<int,int> >* >;
    printAllPathsUtil(array, relevantTraversals, 0, 0, m, n, path, 0);
    delete []path;

    return relevantTraversals;
} 

int** printDiagonal(int M,int N){
    
    int width,height;

        width = N;
        height = M;

    int **array = new int*[height];

    for (int i = 0; i < height; i++)
    {
        array[i]=new int[width];
        memset(array[i],0,width*sizeof(int));
    }

    for (int x = 0; x < M; x++)
    {
        int y = N * x / M;
        array[x][y] = 1;

        //add the neighbor
        if (x+1 < M){
            array[x+1][y] = 1;
        }
        
    }
    
    for (int y = 0; y < N; y++)
    {
        int x = M * y/ N;
        array[x][y] = 1;
        if (x+1 <M){
            array[x+1][y] = 1;
        }
    }
    

    return array;   
}

int main(void){
    int sum = 0;
    vector<vector <pair<int,int> >* > *** vectorArray = new vector<vector <pair<int,int> >* > **[10];
    int **array;
    
    for (int i = 0; i < 10; i++)
    {
        vectorArray[i] = new vector<vector <pair<int,int> >* > *[10];
    }

    for (int i = 0; i < 10; i++)
    {
        //create the three ones above
        for (int j = -3; j <= 3; j++)
        {
            if ((i + j >= 0) && (i + j < 10)){
                array = printDiagonal(i + j + 1, i + 1);
                cout << endl;
                vectorArray[i+j][i] = printAllPaths(array, i + j + 1, i + 1);
                cout << "Size of the vectors: " << vectorArray[i+j][i]->size() << endl;
                sum += vectorArray[i+j][i]->size();

                for (int k = 0; k < i+j+1; k++)
                {
                    delete []array[k];
                }
                delete []array;
                
            }
            
        }        
    }
    
    cout <<"TOTAL SUM " <<sum <<endl;
    for (int i = 0; i < 10; i++)
    {
        //create the three ones above
        for (int j = -3; j <= 3; j++)
        {
            if ((i + j >= 0) && (i + j < 10)){
                //delete the tables
            while (!vectorArray[i+j][i]->empty())
            {

                delete vectorArray[i+j][i]->back();
                vectorArray[i+j][i]->pop_back();
            }
            delete vectorArray[i+j][i];
            }
            
        }        
    }

    for (int i = 0; i < 10; i++)
    {
        delete [] vectorArray[i];
    }
    delete []vectorArray;
    
    return 0;
    
}
