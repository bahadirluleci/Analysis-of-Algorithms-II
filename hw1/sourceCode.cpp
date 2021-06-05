/*
 Student name: Bahadır Lüleci
 Student number: 504201511
 */
// IMPORTANT NOTE: When executing the code, the -std=c++11 flag must be used.
// main.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>
#include <unordered_map>
#include <queue>
#include <stack>
#include <fstream>
#include <chrono>
#include <math.h>

using namespace std;

int globalCounter = 0;    //counts the count of element in the tree
int globalDFSCounter = 0; //counts number of node with DFS traveling until finding a correct result
int globalBFSCounter = 0; //counts number of node with BFS traveling until finding a correct result
//store first letters of inputs
vector<char> firstLetters;
string input_one; //it takes first input of calculation
string input_two; //it takes second input of calculation
string output;    //it takes output of calculation




class Tree
{
public:

    // Structure of a node of an n-ary tree
    struct Node
    {
        vector<pair<char, int > > key;
        bool visited;
        vector<Node*> child;
    };

    struct Node* root;

    Node* newNode(vector<pair<char, int > > key);
    vector<int> availableNumbers(vector<pair<char, int > > parentNodeData);
    vector<pair<char, int > > createKeyVector(vector<char> letters, vector<int> values);
    void displayData(vector<pair<char, int > > retVect);
    void  buildTree(struct Tree::Node* root );
    vector<pair<char, int>> DFS(struct Tree::Node* root);
    vector<pair<char, int>> BFS(struct Tree::Node* root);
    bool solution_finder(vector<pair<char, int > > parentNodeData);
    int get_val(char x, vector<pair<char, int >> NodeData);
};


// Utility function to create a new tree node
Tree::Node* Tree::newNode(vector<pair<char, int > > key) //it is the function required to generate each new node.
{
    Tree::Node* temp = new Tree::Node;
    temp->key = key;
    return temp;
}

vector<int> Tree::availableNumbers(vector<pair<char, int >> parentNodeData)
{
    //This function takes the parent node's data and determines what kind of data can be given to children nodes.
    //In retVect, int values that can come to distinct letters in children are kept in vector.
    vector<int> retVect;

    bool flag = false;
    for (int i = 0; i < 10; ++i) {
        flag = false;
        
        for (int j = 0; j < parentNodeData.size(); ++j)
        {
            if (i == parentNodeData[j].second)
                flag = true;
            if (i == 0 && count(firstLetters.begin(), firstLetters.end(), parentNodeData[j].first) && parentNodeData[j].second == -1){
                flag = true;
            }
        }
        if (!flag) {
            retVect.push_back(i);
        }
    }
    return retVect;
}

vector<pair<char, int > > Tree::createKeyVector(vector<char> letters, vector<int> values) {
    //This function is for initialising the vector pair in the first root. The char and int values that come into it make a pair.
    vector<pair<char, int > > retVect;

    for (int i = 0; i < letters.size(); ++i) {
        retVect.push_back(make_pair(letters[i], values[i]));
    }
    return retVect;
}

void Tree::displayData(vector<pair<char, int > > retVect) {
    for (pair<char, int> oneVal : retVect) {
        cout << oneVal.first << ": :" << oneVal.second << "  ";
    }
    cout << endl;
}


void  Tree::buildTree(struct Tree::Node* root ) {
    //base case: if all numbers are assigned there is no deeper leaf
    int nextChar = 0;
    for (pair<char,int> oneVal : root->key) {
        if (oneVal.second == -1) {
            goto keepMoving;
        }
        nextChar++;

    }
    return ;

    keepMoving:
    root->visited = false;  //For each node added, the visited value is initiliaze false.

        vector<int> availNum = availableNumbers(root->key);

        for (int i = 0; i < availNum.size(); ++i) {
                vector<pair <char, int> > childData = root->key;
                childData[nextChar].second = availNum[i];
                (root->child).push_back(newNode(childData));
        }

        for (int i = 0; i < root->child.size(); ++i) {
            globalCounter++;
//            displayData(root->child[i]->key); // its for checking for the build functions is working.
            buildTree(root->child[i]);
        }

    return;
}

vector<pair<char, int>> Tree::DFS(struct Tree::Node* root){
    //this is the function for the Depth First Search Algorithm
    vector<pair<char, int>> a;
    stack<struct Tree::Node*> s; // Create a stack for DFS
    s.push(root); // Push the current source node.
    while (!s.empty()) {
        struct Tree::Node* top = s.top();
        s.pop(); // Pop a node from stack
//        displayData(top->key);
        globalDFSCounter++; //Number of the visited nodes for DFS
        if(solution_finder(top->key)){ //It checks top node is the correct answer
            a = top->key;
            return a; //if its true then quit from function with vector<pair<char, int>> a
        }
        if(top->visited == false){
            top->visited = true;
        }
        for (int i = 0; i < top->child.size(); ++i) {
            // If a child has not been visited, then push it to the stack.
            if(!(top->child[i]->visited))
                s.push(top->child[i]);
        }
    }
    return a;
}

vector<pair<char, int>> Tree::BFS(struct Tree::Node* root){
    //this is the function for the Breadht First Search Algorithm
    vector<pair<char, int>> a;
    queue<struct Tree::Node*> q; //Create a queue for BFS
    root->visited = true;
    q.push(root); // push the current source node
    while(!q.empty())
    {
        const struct Tree::Node* currentNode = q.front();
        q.pop();
//        displayData(currentNode->key);
        globalBFSCounter++; //Number of the visited nodes for BFS
        if(solution_finder(currentNode->key)){ //It checks top node is the correct answer
            a = currentNode->key;
            return a; //if its true then quit from function with vector<pair<char, int>> a
        }
        
        const vector<struct Tree::Node*> currentChildren = currentNode->child;
        // Get all children nodes oIf they have not been visited,
        // Then mark it visited and push it
        for (int i = 0; i < currentChildren.size(); ++i) {

            if(currentChildren[i]->visited == false){
                currentChildren[i]->visited = true;
                q.push(currentChildren[i]);
            }
//            displayData(root->child[i]->key);
        }

    }
    return a;
}

int Tree::get_val(char x, vector<pair<char, int >> NodeData){
    // this function get val of distinct letter
    int val = -1;
    for(int i=0; i<NodeData.size();i++){
        if(x==NodeData[i].first)
            val = NodeData[i].second;
    }
    return val;
}

bool Tree::solution_finder(vector<pair<char, int >> NodeData){
    // this function find the NodeData has the true solution or not
    // If it has true solution then return true else it returns false.
    int input_one_local = 0;
    int input_two_local = 0;
    int output_local = 0;
    if(get_val(input_one[0], NodeData)== 0) //If inputs first letter is 0 then solution is false
        return false;
    if(get_val(input_two[0], NodeData)== 0) //If inputs first letter is 0 then solution is false
        return false;
    if(get_val(output[0], NodeData)== 0)    //If output first letter is 0 then solution is false
        return false;
    
    for (int i=0; i<input_one.size(); i++){
        if(get_val(input_one[i], NodeData)== -1)
            return false;
        input_one_local += get_val(input_one[i], NodeData)*pow(10,input_one.size()-i-1);
    }
    for (int i=0; i<input_two.size(); i++){
        if(get_val(input_two[i], NodeData)== -1)
            return false;
        input_two_local += get_val(input_two[i], NodeData)*pow(10,input_two.size()-i-1);
    }
    for (int i=0; i<output.size(); i++){
        if(get_val(output[i], NodeData)== -1)
            return false;
        output_local += get_val(output[i], NodeData)*pow(10,output.size()-i-1);
    }
    if(input_one_local + input_two_local == output_local) // check total of input equal to output or not
        return true;
    else
        return false;
}

int main(int argc, char** argv)
{
    //It starts running time
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    Tree tree;

    if (argc != 6) {
        cout << "enter the correct argument count!"  <<endl;
        return -1;
    }
    input_one = string(argv[2]);
    input_two = string(argv[3]);
    output = string(argv[4]);
    string wholeLetters = string(argv[2]) + string(argv[3]) + string(argv[4]);
    firstLetters.push_back(string(argv[2]).front());
    if (!count(firstLetters.begin(), firstLetters.end(), string(argv[3]).front())) {
        firstLetters.push_back(string(argv[3]).front());
    }

    if (!count(firstLetters.begin(), firstLetters.end(), string(argv[4]).front())) {
        firstLetters.push_back(string(argv[4]).front());}

    vector<char> letters;

    for (char key : wholeLetters) {

        if (!count(letters.begin(), letters.end(), key)) {
            letters.push_back(key);
        }

    }

    vector<int> initialNumbers(letters.size(),-1);
    vector<pair<char, int>> res;
    tree.root = tree.newNode(tree.createKeyVector(letters,initialNumbers));
    tree.root->visited = false;
    tree.buildTree(tree.root);

    if(string(argv[1])== "DFS")
        res = tree.DFS(tree.root);
    else if (string(argv[1])== "BFS")
        res = tree.BFS(tree.root);
    else
        return -1;
    
    //these operations for output solution text file
    
    fstream new_file;
    new_file.open(string(argv[5])+".txt", ios::out);
    if(!new_file)
    {
    cout<<"File creation failed";
    }
    else
    {
    new_file<<"\t0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";    //Writing to file
        for(int i=0; i<res.size(); i++){
            new_file<<res[i].first<<"\t";
            for(int j=0; j<res[i].second; j++){
                new_file<<".\t";
            }
            new_file<<"1\t";
            for(int k=0; k<(9-res[i].second); k++){
                new_file<<".\t";
            }
            new_file<<"\n";
        }
    new_file.close();
    }
    
    //Running time is ends
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); //calculate running time
    
    cout << "Algorithm: " << string(argv[1]) << endl;
    if(string(argv[1])=="DFS")
        cout << "Number of the visited nodes: " << globalDFSCounter <<endl;
    if(string(argv[1])=="BFS")
        cout << "Number of the visited nodes: " << globalBFSCounter <<endl;
    cout << "Maximum number of nodes kept in the memory: " << globalCounter << endl;
    cout << "Running time: " << setprecision(2) << duration.count() * (1e-6) << " seconds" << endl;
    cout << "Solution: ";
    for(int i=0; i<res.size(); i++){
        cout << res[i].first << ":" << res[i].second;
        if(res.size()-1 != i)
            cout << ", ";
    }
    cout << endl;

    return 0;

}
