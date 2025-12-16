#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>

typedef long long ll;

using namespace std;

int dfs(string node, unordered_map<string, vector<string>> &graph, unordered_map<string, bool> &visited){
    if(node=="out") return 1;

    visited[node] = true;
    
    int ways = 0;
    for(auto next: graph[node]){
        if(!visited[next])
        ways += dfs(next, graph, visited);
    }

    visited[node] = false;
    
    return ways;
}

// Memoization with state
map<tuple<string, bool, bool>, ll> memo;

ll dfs2(string node, bool hasdac, bool hasfft, unordered_map<string, vector<string>> &graph, unordered_map<string, bool> &visited){
    // Update flags based on current node
    if(node == "fft") hasfft = true;
    if(node == "dac") hasdac = true;

    if(node == "out") {
        return (hasdac && hasfft) ? 1 : 0;
    }

    // Memoization key
    auto key = make_tuple(node, hasdac, hasfft);
    if(memo.count(key)) return memo[key];

    visited[node] = true;
    
    ll ways = 0;
    for(auto next: graph[node]){
        if(!visited[next]){
            ways += dfs2(next, hasdac, hasfft, graph, visited);
        }
    }

    visited[node] = false;
    
    return memo[key] = ways;
}

void part1(ifstream &inputFile)
{
    string line;
    ll res = 0;
    unordered_map<string, vector<string>> graph;
    while (getline(inputFile, line))
    {
        int pos = line.find(':');

        string u = line.substr(0, pos);

        line = line.substr(pos+1);

        stringstream ss(line);
        string v;

        while(getline(ss, v, ' ')){
            graph[u].push_back(v);
        }
    }
    
    string start = "you";
    unordered_map<string, bool> visited;
    res = dfs(start,graph, visited);

    cout<<"\nResult : "<<res;
}


void part2(ifstream &inputFile)
{
    string line;
    ll res = 0;
    unordered_map<string, vector<string>> graph;
    while (getline(inputFile, line))
    {
        int pos = line.find(':');

        string u = line.substr(0, pos);

        line = line.substr(pos+1);

        stringstream ss(line);
        string v;

        while(getline(ss, v, ' ')){
            graph[u].push_back(v);
        }
    }
    
    string start = "svr";
    unordered_map<string, bool> visited;
    res = dfs2(start,false,false,graph,visited);

    cout<<"\nResult : "<<res;
}

int main()
{
    ifstream inputFile("day11Input.txt");
    // part1(inputFile);
    part2(inputFile);
    inputFile.close();

    return 0;
}
