#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <deque>
using namespace std;

//Returns bigger number in a vector
int biggestNum(vector <int> &numVec){
    int tempInt = numVec[0];
    int tempPlace;
    for(int i = 0; i<numVec.size();i++){
        if(tempInt < numVec[i]){
            tempPlace = i;
            tempInt = numVec[i];
        }
    }

    return tempPlace;
}
//Returns the first spot in a vector in a vector that does not contain a 0
int startPlace(vector< vector<int> > &matrix, int starting){
    for(int i = 0; i<matrix[starting].size();i++){
        if(matrix[starting][i] != 0){
            return i;
        }
    }

    return 0;
}

//Reduces a 2d vector until it either is completely reduced or cannot be reduced any loner
bool reduceMatrix(vector< vector<int> > &matrix, int p, int r){
    //Saves how many outputs each node has
    vector<int> amount(matrix.size());
    //Saves what nodes have been visited in a cycle
    vector<int> visited;
    //Deques where each node is put when visited
    deque<int>  tempVec(matrix.size());
    int starting;
    int position;
    //Check how many outs each node has
    for(int i = 0; i<matrix.size();i++){
        int tempCounter = 0;
        for(int j=0;j<matrix[i].size();j++){
            if(matrix[i][j] != 0){
                tempCounter++;
            }
        }
        amount[i] = tempCounter;
    }
    //Finds the node with the largest amount of outputs, if there are two with the same amount of outputs, it selected the first one
    starting = biggestNum(amount);
    //Finds what node to start at for graph reduction
    position = startPlace(matrix, starting);

    int tempInt=0;
    int tempCounter=0;
    int starter = starting;
    //loop that goes through the graph and finds a cycle, and removes the cycle from the graph.
    //Loop breaks when either graph can't be fully reduced or when graph is fully reduced
    while(1){
        starting = starter;
        visited.empty();
        //Pushed back nodes that are visited in cycle onto deque
        while(1){
            visited.push_back(starting);
            starting = matrix[starting][tempCounter] - 1;
            //If the next node start with a 0, than there is a deadlock
            if(starting+1 == 0){
                return false;
            }
            //If the next node was the starting node in the cycle, break current loop
            if(visited[0] == starting){
                break;
            }
        }
        //Loops through cycles
        while(1){
            for(int i=visited.size();i>0;i--){
                    tempVec.clear();
                    //copies matrix[visited nodes in path][where node can go] to temp deque
                    //pushes last visited vector's available edges to tempVec deque
                    int visiting;
                    visiting = visited[i-1];
                    for(int j=0;j<visited.size();j++){
                        tempVec.push_back(matrix[visiting][j]);
                    }
                    //remove visited edge from vector of outputs
                    tempVec.pop_front();
                    //if visited edge has only one amount, remove it.
                    //if visited edge has more than one amount, remove the visited edge and break loop
                    if(amount[visiting] == 1){
                        for(int i=0;i<=tempVec.size();i++){
                            matrix[visiting][i] = tempVec[i];
                        }
                    }
                    if(amount[visiting] > 1){
                        for(int i=0;i<=tempVec.size();i++){
                            matrix[visiting][i] = tempVec[i];
                            if(tempVec[i] > p+r){
                                matrix[visiting][i] = 0;
                            }
                        }
                        break;
                    }

            }
            //Check the amount of nodes left in matrix
            int leave=0;
            for(int j=0;j<matrix.size();j++){
                for(int i=0;i<matrix[j].size();i++){
                    leave += matrix[j][0];
                }
            }
            //One node will always be left after removes all cycles. If it is the right node than the graph is deadlock free
            if(leave == matrix[starter][0]){
                cout << "graph is deadlock free" << endl;
                return true;
            }

            break;
        }

    }
}

int main()
{
    bool    isSafe = false;
    string  line;
    string  fileln;
    int  p;
    int  r;

    char  temp1;
    char  temp2;
    vector<vector<int>> graph;
    vector<vector<int>> outs;
    vector<vector<int>> ioTracker;
    int graphSize = 0;
    int counter = 0;
    cout << "Enter file path\n";
    cin  >> fileln;

    ifstream file(fileln);


    while(getline(file,line))
    {
        //Checks for comments and ignores the line if it starts with %
        if(line.at(0) == '%'){
            continue;
        }
        //Check the amount of number of processes
        else if(line.find("num_processes") != -1){
            temp1 = line.at(14);
            p = temp1 - '0';
            counter++;
            continue;
        }
        //Checks the amount of number of resources
        else if(line.find("num_resources") != -1){
            temp2 = line.at(14);
            r = temp2 - '0';
            counter++;
            continue;
        }
        //Resizes the graph and in/out Tracker to size of processes + resources
        else if(counter == 2){
            graph.resize(p+r, vector<int>(p+r));
            ioTracker.resize(p+r,vector<int>(p+r));
            counter++;
        }
        //Check if value is a number
        else if(line.at(0) >= 48 && line.at(0) <= 57 && counter == 0) {
            counter++;
            continue;
        }

        //Converts string to int
        else if(line.at(0) >= 48 && line.at(0) <= 57 && counter == 3){

            int x = 0;
            for(int i = 0; i < line.size(); i=i+2){

                temp1 = line.at(i);
                graph[graphSize][x] = temp1 - '0';
                x++;
            }
            graphSize++;
        }
    }
    //get all the node outs and puts them in ioTracker
    for (int i = 0; i < graph.size(); i++)
    {
        int tempi = 0;
        for (int j = 0; j < graph[i].size(); j++)
        {
            if(graph[i][j] == 1){
                    if(i != j){
                    ioTracker[i][tempi] = j+1;
                    tempi++;
                    }
            }
        }
    }


    //Goes to reduce matrix function. Arguments are the 2d vector containing where each node goes to, the amount of processes, and the amount of resources
    isSafe = reduceMatrix(ioTracker, p, r);

    //Outputs if graph has deadlocks or not
    if(isSafe == false){
        cout << "graph has a deadlock" << endl;
    }
    if(isSafe == true){
        cout << "graph is deadlock free" << endl;
    }








}
