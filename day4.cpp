#include <iostream>
#include <fstream>

using namespace std;

int evaluateGrid(vector<vector<char>> &grid){

    int n = grid.size();
    int m = grid[0].size();
    int res = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(grid[i][j]!='@'){
                continue;
            }
            int count = 0;
            // Eight Directions
            // i , j
            // N i-1
            // S i+1
            // E j+1
            // W j-1
            // NE i-1 j+1
            // NW i-1 j-1
            // SE i+1 j+1
            // SW i+1 j-1

            // Not So Asthetically Pleasing, I know
            if(i-1 >= 0 && grid[i-1][j]=='@'){
                count++;
            }

            if(i+1<n && grid[i+1][j]=='@'){
                count++;
            }

            if(j+1<m && grid[i][j+1]=='@'){
                count++;
            }

            if(j-1>=0 && grid[i][j-1]=='@'){
                count++;
            }

            if(i-1>=0 && j+1<m && grid[i-1][j+1]=='@'){
                count++;
            }

            if(i-1>=0 && j-1>=0 && grid[i-1][j-1]=='@'){
                count++;
            }

            if(i+1<n && j+1<m && grid[i+1][j+1]=='@'){
                count++;
            }

            if(i+1<n && j-1>=0 && grid[i+1][j-1]=='@'){
                count++;
            }

            if(count<4){
                grid[i][j]='x';
                res++;
            }
        }
    }

    return res;
}

void printGrid(vector<vector<char>> &grid)
{
    int n = grid.size();
    int m = grid[0].size();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout<<grid[i][j]<<" ";
        }
        cout<<"\n";
    }
}

int main()
{
    ifstream inputFile("day4Input.txt");
    vector<vector<char>> grid;

    string line;

    while(getline(inputFile, line)){
        vector<char> currLine;
        int n = line.length();
        for(int i=0; i<n; i++){
            currLine.push_back(line[i]);
        }
        grid.push_back(currLine);
    }

    int total = 0;
    int curr = 0;

    // printGrid(grid);
    do{
        curr = evaluateGrid(grid);
        total += curr;
    }while(curr!=0);
    
    cout<<"Final Result : "<<total;

    inputFile.close();

    return 0;
}



