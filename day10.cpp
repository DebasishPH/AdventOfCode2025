#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <z3++.h>

typedef long long ll;

using namespace std;

int find(int idx, int reqXOR, int &currXOR, vector<vector<int>> &buttons){
     // Base case: found solution
     if(currXOR == reqXOR) return 0;
     
     // Base case: no more buttons
     if(idx >= buttons.size()) return INT_MAX;
     // Pick
     int beforeXOR = currXOR;
     for(auto k: buttons[idx]){
        currXOR ^= (1 << k); 
     }
     
     int pickResult = find(idx+1, reqXOR, currXOR, buttons);
     int pick = (pickResult == INT_MAX) ? INT_MAX : 1 + pickResult;
     
     currXOR = beforeXOR;

     int skip = find(idx+1, reqXOR, currXOR, buttons);

     return min(pick, skip);
}


void part1(ifstream &inputFile)
{
    string line;
    ll res = 0;
    while (getline(inputFile, line))
    {
        // Read the Required Set Up;
        int pos1 = line.find('[');
        int pos2 = line.find(']');
        string req = line.substr(pos1 + 1, pos2 - 1);
        vector<vector<int>> buttons;
        vector<int> joltages;

        int start = pos2 + 1;
        while (line[start] != '{')
        {
            int pos1 = line.find('(', start);
            if (pos1 == string::npos)
                break;

            int pos2 = line.find(')', pos1);

            string currButtons = line.substr(pos1 + 1, pos2 - pos1 - 1);

            vector<int> button;
            stringstream ss(currButtons);
            string num;
            while (getline(ss, num, ','))
            {
                button.push_back(stoi(num));
            }
            buttons.push_back(button);
            start = pos2 + 1;
        }

        start = line.find('{');
        if (start != string::npos)
        {
            int end = line.find('}',start);
            string jolt = line.substr(start+1, end - start - 1);

            stringstream ss(jolt);
            string num;
            while (getline(ss, num, ','))
            {
                joltages.push_back(stoi(num));
            }
        }

        // cout<<"\nLine : "<<line;
        
        // Use XOR Logic
        int reqXOR = 0;
        for(int i=0; i<req.length(); i++){
            if(req[i]=='#'){
                reqXOR ^= (1 << i);  
            }
        }
        
        int currXOR = 0;
        int currPress = find(0,reqXOR,currXOR,buttons);
        res += currPress;
    }

    cout<<"\nResult : "<<res;
}


// int find2(int idx, vector<int> joltages, vector<vector<int>> &buttons){
//     bool allZero = true;
//     for(auto jolt: joltages){
//         if(jolt > 0){
//             allZero = false;
//             break;
//         }
//     }
//     if(allZero) return 0; 

//     if(idx >= buttons.size()) return INT_MAX;

//     int maxPress = INT_MAX;
//     for(auto pos : buttons[idx]){
//         maxPress = min(maxPress, joltages[pos]);
//     }
    
//     int minCost = INT_MAX;
    
//     // Try pressing 0 to maxPress times
//     for(int press = 0; press <= maxPress; press++){
//         vector<int> newJoltages = joltages;
//         for(auto pos : buttons[idx]){
//             newJoltages[pos] -= press;
//         }
        
//         int result = find2(idx + 1, newJoltages, buttons);
//         if(result != INT_MAX){
//             minCost = min(minCost, press + result);
//         }
//     }
    
//     return minCost;
// }

// void part2(ifstream &inputFile)
// {
//     string line;
//     ll res = 0;
//     while (getline(inputFile, line))
//     {
//         int pos2 = line.find(']');
//         vector<vector<int>> buttons;
//         vector<int> joltages;

//         int start = pos2 + 1;
//         while (line[start] != '{')
//         {
//             int pos1 = line.find('(', start);
//             if (pos1 == string::npos)
//                 break;

//             int pos2 = line.find(')', pos1);

//             string currButtons = line.substr(pos1 + 1, pos2 - pos1 - 1);

//             vector<int> button;
//             stringstream ss(currButtons);
//             string num;
//             while (getline(ss, num, ','))
//             {
//                 button.push_back(stoi(num));
//             }
//             buttons.push_back(button);
//             start = pos2 + 1;
//         }

//         start = line.find('{');
//         if (start != string::npos)
//         {
//             int end = line.find('}',start);
//             string jolt = line.substr(start+1, end - start - 1);

//             stringstream ss(jolt);
//             string num;
//             while (getline(ss, num, ','))
//             {
//                 joltages.push_back(stoi(num));
//             }
//         }

//         cout<<"\nLine : "<<line;
//         int curr = 0;

//         curr = find2(0, joltages, buttons);
//         cout<<"\nCurr : "<<curr;
//         res += curr;
//     }

//     cout<<"\nResult : "<<res;
// }

int solve_with_z3(vector<int>& joltages, vector<vector<int>>& buttons) {
    // Create Z3 context - this is the environment for all Z3 operations
    z3::context ctx;
    
    // Create an optimizer - this will find solutions and minimize objectives
    z3::optimize opt(ctx);
    
    // Create integer variables for each button (how many times we press it)
    vector<z3::expr> press_counts;
    for(int i = 0; i < buttons.size(); i++) {
        // Create variable x0, x1, x2, etc. for each button
        string var_name = "x" + to_string(i);
        press_counts.push_back(ctx.int_const(var_name.c_str()));
        
        // Add constraint: each button press count must be >= 0
        opt.add(press_counts[i] >= 0);
    }
    
    // Add constraints: each joltage must be reduced to exactly 0
    for(int j = 0; j < joltages.size(); j++) {
        // Start with 0 reduction
        z3::expr total_reduction = ctx.int_val(0);
        
        // Add up all button presses that affect this joltage position
        for(int i = 0; i < buttons.size(); i++) {
            // Check if button i affects joltage position j
            bool affects = false;
            for(auto pos : buttons[i]) {
                if(pos == j) {
                    affects = true;
                    break;
                }
            }
            
            if(affects) {
                // Add this button's contribution to the reduction
                total_reduction = total_reduction + press_counts[i];
            }
        }
        
        // Constraint: total reduction must equal the initial joltage value
        // This ensures the joltage becomes 0
        opt.add(total_reduction == joltages[j]);
    }
    
    // Create objective: minimize total button presses
    z3::expr total_presses = ctx.int_val(0);
    for(auto& pc : press_counts) {
        total_presses = total_presses + pc;
    }
    
    // Tell Z3 to minimize this expression
    opt.minimize(total_presses);
    
    // Solve the optimization problem
    if(opt.check() == z3::sat) {
        // Solution found! Get the model (assignment of values)
        z3::model m = opt.get_model();
        
        // Extract the minimum total presses
        int result = m.eval(total_presses).get_numeral_int();
        
        // Optional: print individual button press counts
        // for(int i = 0; i < press_counts.size(); i++) {
        //     cout << "Button " << i << ": " << m.eval(press_counts[i]) << " presses\n";
        // }
        
        return result;
    }
    
    // No solution exists
    return -1;
}

void part2_z3(ifstream &inputFile)
{
    string line;
    ll res = 0;
    while (getline(inputFile, line))
    {
        int pos2 = line.find(']');
        vector<vector<int>> buttons;
        vector<int> joltages;

        int start = pos2 + 1;
        while (line[start] != '{')
        {
            int pos1 = line.find('(', start);
            if (pos1 == string::npos)
                break;

            int pos2 = line.find(')', pos1);

            string currButtons = line.substr(pos1 + 1, pos2 - pos1 - 1);

            vector<int> button;
            stringstream ss(currButtons);
            string num;
            while (getline(ss, num, ','))
            {
                button.push_back(stoi(num));
            }
            buttons.push_back(button);
            start = pos2 + 1;
        }

        start = line.find('{');
        if (start != string::npos)
        {
            int end = line.find('}',start);
            string jolt = line.substr(start+1, end - start - 1);

            stringstream ss(jolt);
            string num;
            while (getline(ss, num, ','))
            {
                joltages.push_back(stoi(num));
            }
        }

        int curr = solve_with_z3(joltages, buttons);
        res += curr;
    }

    cout << "\nResult : " << res << endl;
}

int main()
{
    ifstream inputFile("day10Input.txt");
    // part1(inputFile);
    part2_z3(inputFile);
    inputFile.close();

    return 0;
}
