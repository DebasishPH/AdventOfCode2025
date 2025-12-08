#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_set>

typedef long long ll;

using namespace std;

struct compareCircuits
{
    bool operator()(const vector<string> &c1, vector<string> &c2)
    {
        // Example: Prioritize points with a larger sum of x and y coordinates (max-heap behavior)
        return c1.size() > c2.size();
    }
};

void addToCircuit(vector<unordered_set<string>> &circuits, string &coord1, string &coord2)
{
    int foundIndex = -1;
    // Merge Junctions
    for (int i = 0; i < circuits.size(); i++)
    {
        if (circuits[i].count(coord1) || circuits[i].count(coord2))
        {
            if (foundIndex == -1)
            {
                foundIndex = i;
                circuits[i].insert(coord1);
                circuits[i].insert(coord2);
            }
            else
            {
                for (const auto &coord : circuits[i])
                {
                    circuits[foundIndex].insert(coord);
                }
                circuits.erase(circuits.begin() + i);
                i--;
            }
        }
    }

    if (foundIndex == -1)
    {
        circuits.push_back({coord1, coord2});
    }
}

float calculateDist(float &x1, float &y1, float &z1, float &x2, float &y2, float &z2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    float dist = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

    return dist;
}

void part12(ifstream &inputFile)
{
    string line;
    vector<vector<float>> allcoords;
    while (getline(inputFile, line))
    {
        string coord;
        istringstream iss(line);
        vector<float> coords;
        char del = ',';

        while (getline(iss, coord, del))
        {
            coords.push_back(stof(coord));
        }

        allcoords.push_back(coords);
    }

    // for(auto k: allcoords){
    //     for(auto l: k){
    //         cout<<l<<" ";
    //     }
    //     cout<<"\n";
    // }

    // Identify Pairs of Closest
    // Generate Set
    vector<unordered_set<string>> circuits;
    priority_queue<int, vector<int>> max_pq;
    vector<tuple<float, int, int>> distances;
    set<pair<int, int>> junctionSet;
    int n = allcoords.size();
    int pairs = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            float x1 = allcoords[i][0], y1 = allcoords[i][1], z1 = allcoords[i][2];
            float x2 = allcoords[j][0], y2 = allcoords[j][1], z2 = allcoords[j][2];
            float dist = calculateDist(x1, y1, z1, x2, y2, z2);
            distances.push_back({dist, i, j});
        }
    }

    sort(distances.begin(), distances.end());

    ll lastX1, lastX2;
    bool isfirst = true;

    for (auto &[dist, i, j] : distances)
    {
        if (circuits.size() == 1 && circuits[0].size() == n)
            break;
        // if(pairs>=10) break; for part
        if (!junctionSet.count({i, j}))
        {
            string coord1 = to_string(allcoords[i][0]) + "," + to_string(allcoords[i][1]) + "," + to_string(allcoords[i][2]);
            string coord2 = to_string(allcoords[j][0]) + "," + to_string(allcoords[j][1]) + "," + to_string(allcoords[j][2]);
            addToCircuit(circuits, coord1, coord2);
            junctionSet.insert({i, j});
            junctionSet.insert({j, i});
            lastX1 = allcoords[i][0];
            lastX2 = allcoords[j][0];
            // Note Down the last X1 and X2
            pairs++;

            if (circuits.size() == 1 && circuits[0].size() == n)
            {
                break; // This was the final connection
            }
        }
    }

    for (auto circuit : circuits)
    {
        max_pq.push(circuit.size());
    }

    int i = 1;
    ll res = 1;
    while (i <= 3 && max_pq.size() > 0)
    {
        res *= max_pq.top();
        max_pq.pop();
        i++;
    }

    // for (auto circuit : circuits)
    // {
    //     for (auto ele : circuit)
    //     {
    //         cout << ele << "<->";
    //     }
    //     cout << circuit.size();
    //     cout << "\n";
    // }

    cout << "Result : " << res << "\n";
    cout << "X1:" << lastX1 << " X2:" << lastX2;
    cout << "\nMultiply X1 and X2 :" << lastX1 * lastX2;
}

int main()
{
    ifstream inputFile("day8Input.txt");
    // Both Solutions are in the same function
    part12(inputFile);
    inputFile.close();

    return 0;
}
