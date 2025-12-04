#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream inputFile("day1SampleInput.txt");

    string line;
    int initialVal = 50;
    int res = 0;
    int crossZeros = 0;

    while(getline(inputFile, line)){
        char ch = line[0];
        int val = stoi(line.substr(1));
        int crossZero = 0;
        int prevVal = initialVal;

        if(ch=='R'){
           while(val--){
            initialVal = (initialVal + 1) % 100;
            if(initialVal == 0) crossZero++;
           }
        }
        else if(ch=='L'){
           while(val--){
            initialVal = (initialVal - 1 + 100) % 100;
            if(initialVal == 0) crossZero++;
           }
        }
        
        int newVal = initialVal;
        
        // if(newVal == 0){ 
        //     crossZero += 1;
        // }
        cout<<line<<" ";
        cout<<"PrevVal : "<<prevVal<<" "<<"newVal : "<<newVal<<" ";
        cout<<"CrossZero : "<<crossZero<<"\n";
        
        // if(initialVal==0) res++;
        // crossZeros += crossZero > 0 ? crossZero : 0;
        crossZeros += crossZero;
    }

    cout<<"Result : "<<crossZeros<<"\n";

    inputFile.close();

    return 0;
}



