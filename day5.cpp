#include <iostream>
#include <fstream>
#include <map>

typedef long long ll;

using namespace std;

bool isFresh(unordered_map<long long, long long> &omap, long long ele)
{
    
    for(auto k: omap){
       if(ele >= k.first && ele<=k.second){
         return true;
       }
    }

    return false;
}

void part1(ifstream &inputFile){
    unordered_map<long long, long long> omap;
    string line;
    bool processInput = false;
    int res = 0;

    while(getline(inputFile, line)){
        if(line.length()==0){
            processInput = true;
        }

        if(!processInput){
           int pos = line.find('-');
           long long low = stoll(line.substr(0, pos));
           long long hi = stoll(line.substr(pos+1));
           if(omap.count(low)){
            omap[low] = max(omap[low], hi);
           }
           else {
               omap[low] = hi;
           }
        }
        else if(processInput && line.length()>0){
           bool isGood = isFresh(omap, stoll(line));
           if(isGood) res++;
        }
    } 
    
    cout<<"Total Fresh : "<<res<<"\n";
}

void part2(ifstream &inputFile){
    map<long long, long long> omap;
    string line;
    bool processInput = false;

    unordered_map<int, pair<long long, long long>> mp;
    int i = 0;

    while(getline(inputFile, line)){
        if(line.length()==0){
            processInput = true;
            for(auto k: omap){
                mp[i] = {k.first, k.second};
                i++;
            }
        }

        if(!processInput){
           int pos = line.find('-');
           long long low = stoll(line.substr(0, pos));
           long long hi = stoll(line.substr(pos+1));
           if(omap.count(low)){
            omap[low] = max(omap[low], hi);
           }
           else {
               omap[low] = hi;
           }
        }
    }
    
    vector<pair<ll, ll>> ranges;
    ranges.push_back({mp[0].first, mp[0].second});
    for(i=1; i<mp.size(); i++){
        ll currStart = mp[i].first;
        ll currEnd = mp[i].second;

        if(currStart>=ranges[ranges.size()-1].first && currStart<=ranges[ranges.size()-1].second){
           // Perform Merge
           ranges[ranges.size()-1].first = min(currStart, ranges[ranges.size()-1].first);
           ranges[ranges.size()-1].second = max(currEnd, ranges[ranges.size()-1].second);
        } else {
            ranges.push_back({currStart, currEnd});
        }
    }
    ll res = 0;
    for(auto k: ranges){
        res += k.second - k.first + 1;
    }

    cout<<"Total Res : "<<res;
}

int main()
{
    ifstream inputFile("day5Input.txt");
    // I was initially trying to do a Binary Search on the Sorted Elements
    // That never worked out, so I stuck to Brute Force
    // Alas, Maybe at some point I will figure out what the issue was
    // part1(inputFile); 

    // Part 2 was fairly Straight forward to me, Part 1 took me time
    part2(inputFile);

    inputFile.close();

    return 0;
}



