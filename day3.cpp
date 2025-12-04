#include <iostream>
#include <fstream>

using namespace std;

string solve1(string line){
    int prevDigit = 0;
    int currMax = 0;
    int n = line.length();

    prevDigit = line[0] - '0';
    for(int i=1; i<n; i++){
        int currDigit = line[i] - '0'; 
        int currNum = prevDigit*10 + currDigit;
        // cout<<"currNum : "<<currNum<<" currMax : "<<currMax<<"\n";
        currMax = max(currNum, currMax);
        if(currDigit > prevDigit){
            prevDigit = currDigit;
        }
    }
    cout<<line<<"\n";
    cout<<"Max Number : "<<currMax<<"\n";
    return to_string(currMax);
}

unordered_map<string, long long> mp;

long long find(int pos, int remaining, string &line){
    int n = line.length();

    if(remaining == 0) {
        return 0;
    }
    
    if(pos >= n || (n - pos) < remaining) {
        return LLONG_MIN; 
    }

    string key = to_string(pos) + ":" + to_string(remaining);
    if(mp.count(key)) {
        return mp[key];
    }
    
    long long notPick = find(pos + 1, remaining, line);
    
    long long digitValue = (line[pos] - '0');
    long long multiplier = 1;
    for(int i = 1; i < remaining; i++) {
        multiplier *= 10;
    }
    long long pick = digitValue * multiplier + find(pos + 1, remaining - 1, line);
    
    return mp[key] = max(pick, notPick);
}

int main()
{
    ifstream inputFile("day3Input.txt");

    string line;
    long long res = 0;

    while(getline(inputFile, line)){
        long long curr = 0;
        string s = "";
        int n = line.length();
        curr = find(0, 12, line);
        cout<<"Curr Result : "<<curr<<"\n";
        res += curr;
        mp.clear();
    }
    cout<<"Result : "<<res<<"\n"; 

    inputFile.close();

    return 0;
}



