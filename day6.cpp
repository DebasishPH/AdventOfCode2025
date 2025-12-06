#include <iostream>
#include <fstream>
#include <sstream>

typedef long long ll;

using namespace std;

void part1(ifstream &inputFile)
{
    string line;
    vector<vector<string>> numberGrid;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string number;
        vector<string> currNumList;
        while (ss >> number)
        {
            currNumList.push_back(number);
        }

        numberGrid.push_back(currNumList);
   }

    int n = numberGrid.size();
    int m = numberGrid[0].size();
    ll res = 0;
    string op;

    for (int j = 0; j < m; j++)
    {
        ll curr = 0;
        for (int i = n - 1; i >= 0; i--)
        {
            if (i == n - 1){
               op = numberGrid[i][j];
               if(op=="+"){
                curr = 0;
               } else if(op=="*"){
                curr = 1;
               }
            }
            else
            {
                cout << numberGrid[i][j] << "\n";
                if(op=="+"){
                    curr += stoll(numberGrid[i][j]);
                } else if(op=="*"){
                    curr *= stoll(numberGrid[i][j]);
                }
            }
        }
        res += curr;
    }

    cout<<"Result : "<<res; 
}

void part2(ifstream &inputFile)
{
    string line;
    vector<vector<char>> numberGrid;
    while (getline(inputFile, line))
    {
        string number;
        int i = 0;
        int n = line.length();
        vector<char> chList;
        // Reverse it as we need MSB to LSB
        reverse(line.begin(), line.end());
        while (i<n)
        {
            chList.push_back(line[i]);
            i++;
        }
        numberGrid.push_back(chList);
   }

    int n = numberGrid.size();
    int m = numberGrid[0].size();
    ll res = 0;
    ll curr = 0;
    char op;
    vector<ll> currnums;
    for (int j = 0; j < m; j++)
    {      
        ll num = 0;
        for (int i = 0; i < n; i++)
        {           
            if(numberGrid[i][j]=='+')
            {
                curr = 0;
                op = numberGrid[i][j];
            }
            else if(numberGrid[i][j]=='*'){
                curr = 1;
                op = numberGrid[i][j];
            }
            else if(numberGrid[i][j]=='\0'){
                continue;
            }else{
               if(numberGrid[i][j]!=' ')
               num = num*10 + (numberGrid[i][j] - '0'); 
            }
        }
        if(num!=0){
            currnums.push_back(num);
        }
        else if(num==0){
            if(op=='*'){
                curr = 1;
                for(auto k: currnums){
                    curr *= k;
                }
                res += curr;
            }
            else if(op=='+'){
                curr = 0;
                for(auto k: currnums){
                    curr += k;
                }
                res += curr;
            }
            currnums.clear();  
        } 
    }
    // Add the remaining Numbers
    if(op=='*'){
        curr = 1;
        for(auto k: currnums){
            curr *= k;
        }
        res += curr;
    }
    else if(op=='+'){
        curr = 0;
        for(auto k: currnums){
            curr += k;
        }
        res += curr;
    }

    cout<<"Result : "<<res; 
}

int main()
{
    ifstream inputFile("day6Input.txt");
    // part1(inputFile);

    //PS: I dont want to talk about this code
    part2(inputFile);
    inputFile.close();

    return 0;
}
