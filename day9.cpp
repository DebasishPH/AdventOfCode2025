#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>

typedef long long ll;

using namespace std;

void part1(ifstream &inputFile)
{
    string line;
    vector<pair<int, int>> coords;
    while (getline(inputFile, line))
    {
        int pos = line.find(',');
        int x = stoi(line.substr(0, pos));
        int y = stoi(line.substr(pos + 1));

        coords.push_back({x, y});
    }

    // sort(coords.begin(), coords.end(), [](const pair<int,int> &a, const pair<int,int> &b){
    //     if(a.first == b.first)
    //     return a.second < b.second;

    //     return a.first<b.first;
    // });

    // for(auto k: coords){
    //     cout<<k.first<<" "<<k.second<<"\n";
    // }

    int n = coords.size() - 1;
    ll maxArea = 0;

    // while(i<j){
    //     ll l = abs(coords[j].first - coords[i].first) + 1;
    //     ll b = abs(coords[j].second - coords[i].second) + 1;
    //     maxArea = max(maxArea, l*b);
    //     i++;
    //     j--;
    // }

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            ll l = abs(coords[j].first - coords[i].first) + 1;
            ll b = abs(coords[j].second - coords[i].second) + 1;
            maxArea = max(maxArea, l * b);
        }
    }

    cout << "Max Area : " << maxArea;
}

// This part made me cry as initially I was checking something else
void part2(ifstream &inputFile)
{
    string line;
    vector<pair<int, int>> coordsArr;
    
    while (getline(inputFile, line))
    {
        int pos = line.find(',');
        int col = stoi(line.substr(0, pos));
        int row = stoi(line.substr(pos + 1));
        coordsArr.push_back({col, row});
    }

    int n = coordsArr.size();
    ll maxArea = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int x1 = coordsArr[i].first, y1 = coordsArr[i].second;
            int x2 = coordsArr[j].first, y2 = coordsArr[j].second;
            
            // Skip if not forming a proper rectangle
            if (x1 == x2 || y1 == y2) continue;
            
            // Get rectangle bounds
            int xmin = min(x1, x2), xmax = max(x1, x2);
            int ymin = min(y1, y2), ymax = max(y1, y2);
            
            ll area = (ll)(xmax - xmin + 1) * (ll)(ymax - ymin + 1);
            
            // Only check if area is potentially larger
            if (area <= maxArea) continue;
            
            // Check if any boundary edge intersects this rectangle
            bool valid = true;
            for (int k = 0; k < n; k++)
            {
                int l = (k + 1) % n;  // Next point (wrapping)
                int xk = coordsArr[k].first, yk = coordsArr[k].second;
                int xl = coordsArr[l].first, yl = coordsArr[l].second;
                
                // Get edge bounds
                int x1a = min(xk, xl), x2a = max(xk, xl);
                int y1a = min(yk, yl), y2a = max(yk, yl);
                
                // Check if edge bounding box intersects rectangle interior
                if (x1a < xmax && y1a < ymax && x2a > xmin && y2a > ymin)
                {
                    valid = false;
                    break;
                }
            }
            
            if (valid)
            {
                maxArea = area;
            }
        }
    }

    cout << "Max Area : " << maxArea << "\n";
}

int main()
{
    ifstream inputFile("day9Input.txt");
    // part1(inputFile);
    part2(inputFile);
    inputFile.close();

    return 0;
}
