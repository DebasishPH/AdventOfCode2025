#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

typedef long long ll;

using namespace std;

void part1(ifstream &inputFile)
{
    string line;
    vector<vector<char>> diag;
    while (getline(inputFile, line))
    {
        vector<char> currLine;
        for (auto k : line)
        {
            currLine.push_back(k);
        }
        diag.push_back(currLine);
    }

    // Start the Simulation
    int n = diag.size();
    int m = diag[0].size();
    int splits = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (diag[i][j] == 'S')
            {
                // Start the Beam
                diag[i + 1][j] = '|';
            }
            else if (diag[i][j] == '^')
            {
                if (diag[i - 1][j] == '|')
                {
                    splits++;
                    if (j - 1 >= 0)
                        diag[i][j - 1] = '|';
                    if (j + 1 < m)
                        diag[i][j + 1] = '|';
                }
            }
            else if (i - 1 >= 0 && diag[i - 1][j] == '|')
            {
                diag[i][j] = '|';
            }
        }
    }

    // for(int i=0; i<n; i++){
    //     for(int j=0; j<m; j++){
    //         cout<<diag[i][j];
    //     }
    //     cout<<"\n";
    // }

    cout << "Result : " << splits << "\n";
}

void printDiag(vector<vector<char>> &diag)
{
    int n = diag.size();
    int m = diag[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << diag[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

unordered_map<string, ll> memo;

ll find(int row, int n, vector<vector<char>> &diag)
{
    if (row == n - 1 || row >= n)
    {
        // printDiag(diag);
        return 1;
    }

    int m = diag[row].size();
    
    // Memoize this state
    string state = to_string(row) + ":";
    for (int j = 0; j < m; j++)
    {
        if (row > 0 && diag[row - 1][j] == '|')
        {
            state += to_string(j);
        }
    }
    
    // If found, return the existing result
    if (memo.find(state) != memo.end())
    {
        return memo[state];
    }

    int idx = 0;
    int lidx = 0;
    bool found = false;
    for (int j = 0; j < m; j++)
    {
        if (diag[row][j] == '^' && diag[row - 1][j] == '|')
        {
            idx = j;
            found = true;
            break;
        }
        else if (diag[row - 1][j] == '|')
        {
            lidx = j;
            break;
        }
    }

    
    ll result = 0;
    if (!found)
    {
        // This is the one line where there are no splitters
        // Forward the beam and move to next line
        diag[row][lidx] = '|';
        result = find(row + 1, n, diag);
        diag[row][lidx] = '.';
    }
    else
    {
        // send it to left
        ll left = 0;
        if (row + 1 < n && idx - 1 >= 0)
        {
            diag[row][idx - 1] = '|';
            left = find(row + 1, n, diag);
            diag[row][idx - 1] = '.';
        }

        // send it to right
        ll right = 0;
        if (row + 1 < n && idx + 1 < m)
        {
            diag[row][idx + 1] = '|';
            right = find(row + 1, n, diag);
            diag[row][idx + 1] = '.';
        }

        result = left + right;
    }

    memo[state] = result;
    return result;
}

void part2(ifstream &inputFile)
{
    string line;
    vector<vector<char>> diag;
    int Sidx = 0;
    while (getline(inputFile, line))
    {
        vector<char> currLine;
        int len = line.length();
        for (int i = 0; i < len; i++)
        {
            if (line[i] == 'S')
                Sidx = i;
            currLine.push_back(line[i]);
        }
        diag.push_back(currLine);
    }

    // Start the Simulation
    // The idea simple, we just need to simulate
    // what will happenn if I send the beam to the right or to the left

    // First set the start beam
    diag[1][Sidx] = '|';

    int n = diag.size();
    int m = diag[0].size();

    ll res = find(2, n, diag);

    cout << "Result : " << res;
}

int main()
{
    ifstream inputFile("day7Input.txt");
    // part1(inputFile);
    part2(inputFile);
    inputFile.close();

    return 0;
}
