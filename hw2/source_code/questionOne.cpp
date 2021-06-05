// Student name: <Bahadır Lüleci>
// Student number: <504201511>
//question 1
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;
# define INF 0x3f3f3f3f // in general infinity is setting to 0x3f3f3f3f.
//defining of pair templates
typedef pair<int, int> Pair_I_I;
typedef pair<pair<string,int>, int> PP_SI_I;

//@begin get_val: the function which gets unique_id of vertices from name of vertices
int get_val(string x, vector<pair<string,int>> pairs){
    for(int i=0; i<int(pairs.size()); i++){
        if(pairs[i].first==x)
            return pairs[i].second;
    }
    return 1;
}
//@end get_val
//@begin get_name: the function which gets name of vertices from the unique id of vertices
string get_name(int x, vector<pair<string,int>> pairs){
    for(int i=0; i<int(pairs.size()); i++){
        if(x== -1)
            return "None";
        if(pairs[i].second==x)
            return pairs[i].first;
    }
    return "None";
}
//@end get_name

class Graph
{
    int V;    // number of vertices
    //*adj stores vertex and weight pair for every edge((str:name,int:unique id), int weight)
    list< pair<pair<string,int>, int>> *adjList;

public:
    Graph(int V);  // Constructor
    // function to add an edge to graph
    void addEdge(pair<string,int> source, pair<string,int> destination, int weight);
    int get_weight(int source, int destination);

    // find MST using Prim's algorithm. It also prints MST.
    void FindPrimMST(string x, vector<pair<string,int>> pairs, pair<string,int> shortestCh, pair<string,int> hippodrome, vector<pair<string, string>> checker);

};

// Allocates memory
Graph::Graph(int V)
{
    this->V = V;
    adjList = new list<PP_SI_I> [V];
}
//@begin addEdge:add adges to the Adjacency List
void Graph::addEdge(pair<string,int> source, pair<string,int> destination, int weight)
{
    adjList[source.second].push_back(make_pair(destination, weight));
    adjList[destination.second].push_back(make_pair(source, weight));
}
//@end addEdge
//@begin get_weight: returns weight of edge with their unique id's
int Graph::get_weight(int source, int destination){
    list< pair<pair<string,int>, int>>::iterator i;
    for (i = adjList[source].begin(); i != adjList[source].end(); ++i)
    {
        if(destination == (*i).first.second)
            return (*i).second;
    }
    return -1;
}
//@end get_weight
// Prints shortest paths from src to all other vertices
void Graph::FindPrimMST(string x, vector<pair<string,int>> pairs, pair<string,int> shortestCh, pair<string,int> hippodrome, vector<pair<string, string>> checker)
{
    // Create a priority queue to store vertices that are inMST
    priority_queue< Pair_I_I, vector <Pair_I_I> , greater<Pair_I_I> > pq;

    int src = get_val(x, pairs); // taking id of source
    int temp_ch = get_val(shortestCh.first, pairs); //gets unique id of closest church
    int temp_hi = get_val(hippodrome.first, pairs); //get hippodrome id
    // To keep track of vertices included in MST
    vector<bool> inMST(V, false);
    // key is a vector for keys and initialize all to infinitie
    vector<int> key(V, INF);
    // To store parent array which in turn store MST
    vector<int> parent(V, -1);
    // Insert source itself in priority queue and initialize
    pq.push(make_pair(0, src));
    key[src] = 0;

    /*stop looping if pq empty*/
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;  // Include vertex in MST
        //@begin of adj list loop
        list <pair<pair<string,int>, int>>::iterator i;
        for (i = adjList[u].begin(); i != adjList[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent of u.
            int v = (*i).first.second;
            int weight = (*i).second;
            if( v == temp_ch){ // temp_di == id of shortest ch
                weight = -3;  //val is set to -3 temperarly because it should connect firsty
            }
            if( v == temp_hi){ // temp_hi == id of hippodrome
                weight = -2;  //val is set to -2 temperarly because it should connect secondly
            }

            //If V is not at MST and the weight of (u, v) is less than the current key of v
            if (inMST[v] == false && key[v] > weight)
            {
                // Updating key of v
                key[v] = weight;
                pq.push(make_pair(key[v], v));
                parent[v] = u; //parent list is importent for keeping track of MST
            }
        }
        //@end of adj list loop
    }
    string pair1,pair2;
    vector < pair<pair<string,string>, int>> pair_holder;
    int wei;
    // Print edges of MST using parent array
    int weight_mst = 0; // it holds total weight of MST
    //@start: add pairs to the pair holder with weights
    for (int i = 1; i < V; ++i){
        //if parent of vertice is not -1 (init val) add them to pair_holder
        if(parent[i] != -1){
            weight_mst += get_weight(parent[i], i);
            pair1 = get_name(parent[i], pairs) ;
            pair2 = get_name(i, pairs);
            wei = get_weight(parent[i], i);
            pair_holder.push_back(make_pair(make_pair(pair1, pair2), wei));
        }
    }
    //@end pairs added to pair holder
    //@start: lambda function for sorting pair holder
    sort( pair_holder.begin(), pair_holder.end(),[]( const std::pair<pair<string,string>, int> &p1, const std::pair<pair<string,string>, int> &p2 )
    {
        return p1.second < p2.second;

    } );
    //@end: lambda sorting function
    // i and j are iterators for looping
    vector < pair<pair<string,string>, int>>::iterator i;
    vector <pair<string,string>>::iterator j;
    //@start: loop for printing pairs in true order
    for (i = pair_holder.begin(); i != pair_holder.end(); ++i)
    {
        for (j = checker.begin(); j != checker.end(); ++j){
            if((*j).first == (*i).first.first && (*i).first.second == (*j).second ){
                cout << (*i).first.first << " " << (*i).first.second << " " << (*i).second << "\n";
            }
            else if ((*j).first == (*i).first.second && (*i).first.first == (*j).second ){
                cout << (*i).first.second << " " << (*i).first.first << " " << (*i).second << "\n";
            }
            else
                continue;
        }
    }
    //@end: printing loop ends.
    //print total weight of MST.
    cout << weight_mst << "\n";
}

// Driver program
int main()
{
    //@start: take the file name and open
    string fname;
    cin >> fname;
    ifstream open_file_for_vercite_count(fname);
    //@end
    string line;
    int vertices_counter = 0;
    vector<string> set_of_nodes;

    vector<pair<string, string>> checker; //checker holds true order of vertices in the edges
    //@start: add edges to checker vector and add all nodes to set_of_nodes vector
    while(getline(open_file_for_vercite_count, line)){
        stringstream ss(line);
        string source, destination;
        int weight;
        std::getline(ss, source, ',');
        std::getline(ss, destination, ',');
        ss >> weight;

        set_of_nodes.push_back(source);
            set_of_nodes.push_back(destination);
            checker.push_back(make_pair(source, destination));
    }
    //@end: loop.
    //@start: looping. letters hold set of nodes after the looping
    vector<string> letters;
    for (string key : set_of_nodes) {
        if (!count(letters.begin(), letters.end(), key)) {
            letters.push_back(key);
            vertices_counter ++;
        }
    }//@end:looping
    vector<pair<string,int>> pairs;
    for(int i=0; i<int(letters.size()); i++){
        pairs.push_back(make_pair(letters[i], i));
    }
    //@start:temp val holding operations
    int temp_w = 0;
    string temp_d, temp_h;
    vector<pair <string,int>> ch_reg;
    pair<string,int> hippodrome;
    //@end
    //@start: create a graph object and add edges into it.
    //check constraints while adding edges.
    Graph graph(vertices_counter);
    ifstream city_plan2(fname);
    while(getline(city_plan2, line)){
        stringstream ss(line);
        string source;
        string destination;
        int weight;

        std::getline(ss, source, ',');
        std::getline(ss, destination, ',');
        // Read the integers using the operator >>
        ss >> weight;
        char first_c_source[2] = {source[0] ,source[1]};
        char first_c_dest[2] = {destination[0] ,destination[1]};
        //The great palace must be directly connected to at least one church
        if((source[0] == 'G' && source[1] == 'P' && destination[0] == 'C' && destination[1] == 'h') || (source[0] == 'C' && source[1] == 'h' && destination[0] == 'G' && destination[1] == 'P') ){
            temp_w = weight;
            temp_d = destination;
            ch_reg.push_back(make_pair(temp_d, temp_w));
        }
        //take val of hippodrome
        if((source[0] == 'G' && source[1] == 'P' && destination[0] == 'H' && destination[1] == 'i')||
           (source[0] == 'H' && source[1] == 'i' && destination[0] == 'G' && destination[1] == 'P')){
            temp_w = weight;
            temp_h = destination;
            hippodrome = make_pair(temp_h, temp_w);
        }
        //there should not be any direct connections between the housed of important people.
        if(!(first_c_dest[0] == 'H' && first_c_dest[1] == 'p' && first_c_source[0] == 'H' && first_c_source[1] == 'p')){
            if(!(first_c_dest[0] == 'B' && first_c_source[0] == 'H' && first_c_source[1] == 'i' )){
                if(!(first_c_source[0] == 'B' && first_c_dest[0] == 'H'&& first_c_dest[1] == 'i' )){
                graph.addEdge(make_pair(source, get_val(source, pairs)), make_pair(destination, get_val(destination, pairs)), weight);
                }
            }
        }
    }
    //@end of adding edges.
    //@start: sort ch list for finding shortest ch.
    sort( ch_reg.begin(), ch_reg.end(),[]( const std::pair<string, int> &p1, const std::pair<string, int> &p2 )
    {
        return p1.second < p2.second;

    } );
    //@end
    pair<string,int> shortestCh = ch_reg[0]; // represents shortest church
    string startVertex = "GP"; //starting vertex
    graph.FindPrimMST(startVertex, pairs, shortestCh, hippodrome, checker); //function for finding and printing MST

    return 0;

}
