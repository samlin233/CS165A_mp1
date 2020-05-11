#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

map<string,int> trainpositive;
map<string,int> trainnegative;
int totalcommon =0;
int posicommon =0;
int negacommon =0;
int traintotal=0;
int testtotal =0;
int numtrainpositive =0;
int numtrainnegative =0;
double trainnegarate =0;
double trainposirate =0;
int testcorrect =0;
int smoothing = 10;

vector<bool> testresult;
int main(int argc, char* argv[]){

    ifstream trainfile;
    trainfile.open(argv[1]);
    if(trainfile.is_open()){
        while(trainfile.good()){
            
            string line;
            getline(trainfile,line);

            int begin = 0;
            int end = 0;
            bool posi = 0;

            totalcommon ++;
            if(line.at(line.length()-1)=='1'){
                posi = 1;
                posicommon ++;
            }
            else
            {
                posi = 0;
                negacommon ++;
            }
            
            for(int i=0;i<line.length()-2;i++){
                end = i;
                string temp = line.substr(begin,end-begin);
                begin = i+1;
                if(line.at(i)==' '&&posi==1){
                    if(trainpositive.find(temp) != trainpositive.end()){
                        trainpositive[temp] ++;
                    }
                    else{
                        trainpositive.insert({temp,1});
                    }
                    traintotal += 1;
                    numtrainpositive += 1;
                }
                if(line.at(i)==' '&&posi==0){
                    if(trainnegative.find(temp) != trainnegative.end()){
                        trainnegative[temp] ++;
                    }
                    else{
                        trainnegative.insert({temp,1});
                    }
                    traintotal += 1;
                    numtrainnegative += 1;
                }
            }
        }
    }
    //不确定使用这个还是下方的(negacommon/totalcommon)
    trainposirate = numtrainpositive/traintotal;
    trainnegarate = numtrainnegative/traintotal;
    ifstream testfile;
    testfile.open(argv[2]);
    if(testfile.is_open()){
        while(testfile.good()){
            string line;
            getline(trainfile,line);

            int begin = 0;
            int end = 0;
            vector<string> tempstr;
            
            for(int i=0;i<line.length()-2;i++){
                if(line.at(i) == ' '){
                    end = i;
                    tempstr.push_back(line.substr(begin,end-begin));
                    begin = i+1;
                }
            }
            double probofposi =0;
            double probofnega =0;
            for(int j=0;j<tempstr.size();j++){
                probofposi *= double((trainpositive[tempstr[j]]+smoothing)/(trainpositive[tempstr[j]]+trainnegative[tempstr[j]]+smoothing)
                              *(trainpositive[tempstr[j]]+trainnegative[tempstr[j]]+smoothing)/traintotal/(posicommon/totalcommon))
                probofnega *= double((trainnegative[tempstr[j]]+smoothing)/(trainpositive[tempstr[j]]+trainnegative[tempstr[j]]+smoothing)
                              *(trainpositive[tempstr[j]]+trainnegative[tempstr[j]]+smoothing)/traintotal/(negacommon/totalcommon))
            }

            if(probofposi>probofnega){
                testresult.push_back(1);
                cout<<1<<endl;
            }
            else{
                testresult.push_back(0);
                cout<<0<<endl;
            }
        }
    }

    return 0;
}
