#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <numeric>

typedef long long ll;

using namespace std;

vector<vector<char>> flipHorizontal(const vector<vector<char>> &shape)
{
    vector<vector<char>> flipped = shape;
    for (auto &row : flipped)
    {
        reverse(row.begin(), row.end());
    }
    return flipped;
}

vector<vector<char>> rotate90(const vector<vector<char>> &shape)
{
    int rows = shape.size();
    int cols = shape[0].size();
    vector<vector<char>> rotated(cols, vector<char>(rows));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }
    return rotated;
}

// Generate all unique orientations once
vector<vector<vector<char>>> getAllOrientations(const vector<vector<char>> &shape)
{
    set<vector<vector<char>>> uniqueOrientations;
    
    // Original and rotations
    auto curr = shape;
    for (int i = 0; i < 4; i++)
    {
        uniqueOrientations.insert(curr);
        curr = rotate90(curr);
    }
    
    // Flipped and rotations
    auto flipped = flipHorizontal(shape);
    curr = flipped;
    for (int i = 0; i < 4; i++)
    {
        uniqueOrientations.insert(curr);
        curr = rotate90(curr);
    }
    
    return vector<vector<vector<char>>>(uniqueOrientations.begin(), uniqueOrientations.end());
}

bool canPlaceAt(const vector<vector<char>> &shape, vector<vector<char>> &grid, int startR, int startC)
{
    int gridRows = grid.size();
    int gridCols = grid[0].size();
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();

    // Check if shape goes out of bounds
    if (startR + shapeRows > gridRows || startC + shapeCols > gridCols)
    {
        return false;
    }

    // Check if all '#' cells in shape can be placed (no overlap with existing '#')
    for (int i = 0; i < shapeRows; i++)
    {
        for (int j = 0; j < shapeCols; j++)
        {
            if (shape[i][j] == '#')
            {
                if (grid[startR + i][startC + j] != '.')
                {
                    return false; // Cell already occupied
                }
            }
        }
    }

    // Actually place the shape
    for (int i = 0; i < shapeRows; i++)
    {
        for (int j = 0; j < shapeCols; j++)
        {
            if (shape[i][j] == '#')
            {
                grid[startR + i][startC + j] = '#';
            }
        }
    }

    return true;
}

void removePlacement(const vector<vector<char>> &shape, vector<vector<char>> &grid, int startR, int startC)
{
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    
    for (int i = 0; i < shapeRows; i++)
    {
        for (int j = 0; j < shapeCols; j++)
        {
            if (shape[i][j] == '#')
            {
                grid[startR + i][startC + j] = '.';
            }
        }
    }
}

bool solve(vector<vector<char>> &grid, vector<int> &req, 
           map<int, vector<vector<vector<char>>>> &allOrientations)
{
    if (accumulate(req.begin(), req.end(), 0) == 0)
    {
        return true;
    }

    // Find first required shape (optimization: process in order)
    for (int i = 0; i < req.size(); i++)
    {
        if (req[i] > 0)
        {
            int gridRows = grid.size();
            int gridCols = grid[0].size();

            // Try all orientations of this shape
            for (const auto &orientation : allOrientations[i])
            {
                int shapeRows = orientation.size();
                int shapeCols = orientation[0].size();
                
                // Early skip if shape too big
                if (shapeRows > gridRows || shapeCols > gridCols)
                    continue;

                // Try all positions
                for (int r = 0; r <= gridRows - shapeRows; r++)
                {
                    for (int c = 0; c <= gridCols - shapeCols; c++)
                    {
                        if (canPlaceAt(orientation, grid, r, c))
                        {
                            req[i]--;
                            if (solve(grid, req, allOrientations))
                            {
                                return true;
                            }
                            // Backtrack 
                            req[i]++;
                            removePlacement(orientation, grid, r, c);
                        }
                    }
                }
            }
            
            // Couldn't place this required shape
            return false;
        }
    }

    return false;
}

void part1(ifstream &inputFile)
{
    string line;
    ll res = 0;
    unordered_map<int, vector<vector<char>>> shapes;
    vector<pair<int, int>> grids;
    unordered_map<int, vector<int>> gridReqs;
    
    while (getline(inputFile, line))
    {
        int pos = line.find(':');
        int pos2 = line.find('x');
        if (pos != string::npos && pos2 == string::npos)
        {
            int num = stoi(line.substr(0, pos));

            vector<vector<char>> currShape;
            while (getline(inputFile, line))
            {
                if (line.length() == 0)
                {
                    break;
                }

                vector<char> currLine(line.begin(), line.end());
                currShape.push_back(currLine);
            }

            shapes[num] = currShape;
        }
        else if (pos2 != string::npos)
        {
            int p1 = line.find('x');
            int p2 = line.find(':');

            int r = stoi(line.substr(0, p1));
            int c = stoi(line.substr(p1 + 1, p2));

            grids.push_back({r, c});

            string currLine = line.substr(p2 + 2);
            stringstream ss(currLine);
            
            vector<int> reqs;
            int num;
            while (ss >> num)
            {
                reqs.push_back(num);
            }

            gridReqs[grids.size() - 1] = reqs;
        }
    }

    // Pre-compute all orientations for each shape (ONCE)
    map<int, vector<vector<vector<char>>>> allOrientations;
    for (const auto &[id, shape] : shapes)
    {
        allOrientations[id] = getAllOrientations(shape);
    }

    // Process each grid
    for (int i = 0; i < grids.size(); i++)
    {
        int r = grids[i].first;
        int c = grids[i].second;
        vector<int> req = gridReqs[i];

        vector<vector<char>> grid(r, vector<char>(c, '.'));
        cout<<"Solving for Grid : "<<i<<"\n";

        if (solve(grid, req, allOrientations))
            res++;
    }

    cout << "\nResult : " << res;
}

void part2(ifstream &inputFile){
    string line;
    int fit = 0;

    while (getline(inputFile, line))
    {
        size_t xPos = line.find('x');
        size_t colonPos = line.find(':');
        
        if (xPos != string::npos && colonPos != string::npos && xPos < colonPos)
        {
            int w = stoi(line.substr(0, xPos));
            int h = stoi(line.substr(xPos + 1, colonPos - xPos - 1));
            
            int area = (w / 3) * (h / 3);
            
            string boxesPart = line.substr(colonPos + 1);
            stringstream ss(boxesPart);
            int totalBoxes = 0;
            int num;
            
            while (ss >> num)
            {
                totalBoxes += num;
            }
            
            if (totalBoxes <= area)
            {
                fit++;
            }
        }
    }
    
    inputFile.close();
    cout << "Result: " << fit << "\n";
}


int main()
{
    ifstream inputFile("day12Input.txt");
    // part1(inputFile);
    part2(inputFile);
    inputFile.close();

    return 0;
}