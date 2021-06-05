// Student name: <Bahadır Lüleci>
// Student number: <504201511>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <sstream>
#include <cstdio>
using namespace std;
//used for sorting function <alphabetically>
bool compareFunction (string a, string b) {return a<b;}
//return max value
int max_val(int val_1, int val_2, int val_3){
    int temp = val_1;
    if(val_1 < val_2)
        temp = val_2;
    if(val_2 < val_3)
        temp = val_3;
    return temp;
}

/*@Start: MatrixPair class for every pair of word strings */
class MatrixPair{
public:
    string pairOne;
    string pairTwo;
    int score;
    vector<vector<int>> matrixOfPairs;

public:
    MatrixPair(string pairOne, string pairTwo);
    void MatrixFiller(int match_val, int mismatch_penalty, int gap_penalty);
    void ScoreFinder(int match_val);
    void traceBack(int match_val);
};
/*@End: MatrixPair class for every pair of word strings */
/*@Start: MatrixPair is theConstructure of MatrixPair object. Initiliaze object items*/
MatrixPair::MatrixPair(string pairOne, string pairTwo){
    this->score = 0;
    this->pairOne = pairOne;
    int pairOneSize = int(pairOne.size());
    this->pairTwo = pairTwo;
    int pairTwoSize = int(pairTwo.size());
    //create matrix with size of string 1 x string 2
    //and initiliaze it with 0
    this->matrixOfPairs =  vector<vector<int>> ( pairTwoSize + 1 , vector<int> (pairOneSize + 1, 0));
}
/*@End: MatrixPair*/
/*@Start: MatrixFiller assigns a value for each matching character
 If the character matches and the character in the diagonal region also matches, one more of the value in the diagonal region is written into the matrix.*/
void MatrixPair::MatrixFiller(int match_val, int mismatch_penalty, int gap_penalty){
    int left_box, right_box, diagonal;
    for(int i=1; i<int(pairTwo.size())+1; i++){
        for(int j=1; j<int(pairOne.size())+1; j++){
            
            left_box = matrixOfPairs[i][j-1] + gap_penalty;
            right_box = matrixOfPairs[i-1][j] + gap_penalty;
            diagonal = matrixOfPairs[i-1][j-1];
            
            if(pairTwo[i-1] == pairOne[j-1]){
                diagonal += match_val;
                matrixOfPairs[i][j] = max_val(left_box, right_box, diagonal);
                if(matrixOfPairs[i][j] < 0)
                    matrixOfPairs[i][j] =  0;
            }
            else{
                diagonal += mismatch_penalty;
                matrixOfPairs[i][j] = max_val(left_box, right_box, diagonal);
                if(matrixOfPairs[i][j] < 0)
                    matrixOfPairs[i][j] =  0;
            }
        }
    }
}
/*@End: MatrixFiller*/
/*@Start: ScoreFinder finds the largest integer value in each matrix and assigns it to the score variable in the object. */
void MatrixPair::ScoreFinder(int matching_val){
    for(int i=0; i<int(pairTwo.size()+1); i++){
        int it = *max_element(begin(matrixOfPairs[i]), end(matrixOfPairs[i]));
        if(it> score)
            score = it;
    }
    score = score/ matching_val;
}
/*@End: ScoreFinder*/
/*@Start: Traceback does tree things.
 Step 1: Finds how many values in matrix are equal to scor value.There are as many longest common substrings as the number found.
 Step 2: Goes to the areas on the matrix equivalent to the score value and saves them in a vectore until their diagonals are 0. It then saves the characters found in these regions.
 Step 3: Print all Sequence(s) inside it.*/
void MatrixPair::traceBack(int matching_val){
    int count_of_longest_items = 0;
    
    for(int i=0; i<int(pairTwo.size()) + 1; i++){
        for(int j=0; j<int(pairOne.size()) + 1; j++){
            if(matrixOfPairs[i][j] == score*matching_val)
                count_of_longest_items ++;
        }
    }
    vector<vector<char>> tracebackVector(count_of_longest_items);

    int temp_i = 0, temp_j = 0;
    int counter = 0;
    int t_i, t_j;
    
    while(count_of_longest_items != 0){
        for(int i=temp_i; i<int(pairTwo.size()) + 1; i++){
            for(int j=temp_j; j<int(pairOne.size()) + 1; j++){
                if(matrixOfPairs[i][j] == score*matching_val){
                    int k = j-1;
                    t_i = i;
                    t_j = j;
                    while(matrixOfPairs[t_i][t_j] != 0){

                        tracebackVector[counter].push_back(pairOne[k]);
                        k--;
                        t_i--;
                        t_j--;
                    }
                    temp_i = i+1;
                    temp_j = 0;
                    goto finish;
                }
            }
        }
        finish:
        reverse(tracebackVector[counter].begin(), tracebackVector[counter].end());
        counter ++;
        count_of_longest_items --;
    }
    //sort tracebackVector alphabetically and keep only unique items inside it
    sort(tracebackVector.begin(),tracebackVector.end());
    auto iter = std::unique(tracebackVector.begin(), tracebackVector.end());
    tracebackVector.erase(iter, tracebackVector.end());
    for(int i=0; i<int(tracebackVector.size()); i++){
        if(tracebackVector[i].size() == 0)
            tracebackVector.erase (tracebackVector.begin()+i);
    }
    //print substrings
    for(int i=0 ; i<int(tracebackVector.size()); i++){
        cout << "\"";
        for (vector<char>::const_iterator j = tracebackVector[i].begin(); j != tracebackVector[i].end(); ++j)
        {
                cout << *j;
        }
        if(i == (int(tracebackVector.size()) -1))
            cout << "\"";
        else
            cout << "\" ";
    }
        cout << "\n";
}
/*@End: traceBack*/


int main(int argc, const char * argv[]) {
    //default values. It would be more correct to use
    int match_val = 1;
    int mismatch_penalty = -2;
    int gap_penalty = -4;
    //values with arguments
    if (argc == 6) {
        match_val = atoi(argv[3]);
        mismatch_penalty = atoi(argv[4]);
        gap_penalty = atoi(argv[5]);
    }
        
    /* @start: backup cout buffer and redirect to out.txt */
    string out_f_name;
    if(argc < 2)
        out_f_name = "output.txt";
    else
        out_f_name = string(argv[2]);
    ofstream out(out_f_name);
    auto *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());
    /* @end */
    vector<string> inputStrings;
    string fname;
    //get file name into fname
    if(argc < 2)
        fname = "strings.txt";
    else
        fname = string(argv[1]);
    ifstream input_file(fname);
    string line;
    //reading every strings in the file
    while (getline(input_file, line, '\n')) {
        if (!line.empty() && line[line.size() - 1] == '\r')
            line.erase(line.size() - 1);
            
        inputStrings.push_back(line);
    }
    //sort the vector alphabetically
    sort(inputStrings.begin(),inputStrings.end(),compareFunction);
    
    vector<MatrixPair> AllObjectPairs;
    //add all string pairs in object vector
    for (int i=0; i<int(inputStrings.size()); i++){
        for(int j=i+1; j<int(inputStrings.size()); j++)
        AllObjectPairs.push_back(MatrixPair(inputStrings[i], inputStrings[j]));
    }
    //two times loop
    //first time for writing to the file for calico testing
    //second time for printing to console
    for(int j = 0; j<2; j++){
    //@Start: traverse all objects and find their substrings
    for(int i=0; i<int(AllObjectPairs.size()); i++){
        //takes matching point, mismatch_penalty, gap penalty
        AllObjectPairs[i].MatrixFiller(match_val, mismatch_penalty, gap_penalty);
        // takes only matching point
        AllObjectPairs[i].ScoreFinder(match_val);
        cout << AllObjectPairs[i].pairOne << " - "<<AllObjectPairs[i].pairTwo << "\n";
        if(AllObjectPairs[i].score != 0)
            cout << "Score: " << AllObjectPairs[i].score * match_val << " " << "Sequence(s): ";
        else
            cout << "Score: " << AllObjectPairs[i].score * match_val << " " << "Sequence(s):";
        AllObjectPairs[i].traceBack(match_val);
    }
        if(j==0)
            cout.rdbuf(coutbuf); //reset cout buffer
        //after this will be printed on console
    }
     //@End
    return 0;
}
