#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Precompute the repeating Pattern

long long calculate(long long start, long long end){
    long long res = 0;
    while(start <= end){
       string currNum = to_string(start);
       if(currNum.size()%2==0){
          int midPoint = currNum.length()/2;
          string firstHalf = currNum.substr(0, midPoint);
          string secondHalf = currNum.substr(midPoint);

          if(firstHalf == secondHalf){
             cout<<"CurrNum : "<<currNum<<"\n";
             res+=start;
          }
       }
       start++;
    }

    return res;
}

long long calculate2(long long start, long long end){
    long long res = 0;
    while(start <= end){
       string currNum = to_string(start);
       int len = currNum.length();
       for(int patternLen = 1; patternLen <= len/2; patternLen++){
           if(len % patternLen != 0) continue;           
           string pattern = currNum.substr(0, patternLen);
           bool isRepeating = true;
           for(int pos = patternLen; pos < len; pos += patternLen){
              //  cout<<"Current String : " << pattern <<" ";
              //  cout<<"Pattern to Check : " + currNum.substr(pos, patternLen) << "\n";
              // 1111
              // 1 1 11
               if(currNum.substr(pos, patternLen) != pattern){
                   isRepeating = false;
                   break;
               }
           }          
           if(isRepeating){
              //  cout<<"Repeating Pattern : "<<pattern<<" CurrNum : "<<currNum<<"\n";
               res += stoll(currNum);
               break; 
           }
       }
       
       start++;
    }

    return res;
}


int main()
{
    ifstream inputFile("day2.txt");

    string line;
    long long res = 0;

    while(getline(inputFile, line)){
        std::stringstream ss(line); // Create a stringstream from the current line
        std::string segment;
        std::vector<std::string> rowData;

        // Extract segments separated by commas
        while (getline(ss, segment, ',')) {
            // cout<<segment<<" ";
            int pos = segment.find('-');
            long long start = stoll(segment.substr(0, pos));
            long long end = stoll(segment.substr(pos+1));
            // cout<<"start : "<<start<<" "<<"end : "<<end<<" ";
            res += calculate2(start, end);
            // cout<<"\n";
        }
    }

    cout<<"Result : "<<res;

    inputFile.close();

    return 0;
}



