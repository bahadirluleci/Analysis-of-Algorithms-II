// Student name: <Bahadır Lüleci>
// Student number: <504201511>
//question2
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
        if(pairs[i].second==x)
            return pairs[i].first;
    }
    return "None";
}
//@end: get_name
//@start: recursive function for getting_parent
vector<int> get_parent(vector<int> parent, int destination, vector<pair<string,int>> pairs, vector<int> rev){
    //base condition
    if(parent[destination] == 0)
        return rev;
    else
    {
        rev.push_back(parent[destination]);
        return get_parent(parent, parent[destination], pairs, rev);
    }
}
//@end

class Graph
{
    int V;    // number of vertices
    //*adjList stores vertex and weight pair for every edge((str:name,int:unique id), int weight)
    list< pair<pair<string,int>, int>> *adjList;
public:
    Graph(int V);  // Constructor
    // function to add an edge to graph
    void addEdge(pair<string,int> source, pair<string,int> destination, int weight);
    //returns weight on an edge
    int get_weight(int source, int destination);
    // prints shortest path from s
    void shortestPath(int source, int destination, vector<pair<string,int>> pairs);
};

// Allocates memory for adjacency list
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
void Graph::shortestPath(int src, int dest, vector<pair<string,int>> pairs)
{
    // Create a priority queue to store vertices that are being preprocessed
    priority_queue< Pair_I_I, vector <Pair_I_I> , greater<Pair_I_I> > pq;
  
    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<int> dist(V, INF);
    //vector for keeping track of shortest path
    vector<int> parent(V, -1);
    // Insert source itself in priority queue, first distance is 0
    pq.push(make_pair(0, src));
    dist[src] = 0;
  
    /*stop looping if pq empty*/
    while (!pq.empty())
    {
        int u = pq.top().second; // take weight
        pq.pop();
        // 'i' is the iterator which used to get all adjacent vertices of a vertex
        list <pair<pair<string,int>, int>>::iterator i;
        //@begin of adj list loop
        for (i = adjList[u].begin(); i != adjList[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first.second;
            int weight = (*i).second;
  
            //  If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
                parent[v] = u;

            }
        }
        //@end looping
    }
    //@start: printing shortest path
    vector<int> parent_holder;
    cout << get_name(src,pairs) << ' ';
    parent_holder = get_parent(parent, dest, pairs, parent_holder);
    for(int i=int(parent_holder.size()-1); i>=0; i--){
        cout << get_name(parent_holder[i], pairs) << ' ';

    }
    cout << get_name(dest,pairs) << ' ' << dist[dest] << "\n";
    //@end printing
}
  
// Driver program
int main()
{
    //@start: take the file name and open
    string fname;
    cin >> fname;
    ifstream openFileForEnemyDistanceCheck(fname);
    string line;
    //@end
    int vertices_counter = 0;
    vector<string> set_of_nodes; // holds all nodes before setting
    //@start: add nodes to set of nodes
    vector<string> restrictedSpots; //it holds spots where is close distance unit <5
    while(getline(openFileForEnemyDistanceCheck, line)){
        stringstream ss(line);
        string source;
        string destination;
        int weight;
        std::getline(ss, source, ',');
        std::getline(ss, destination, ',');
        ss >> weight;
        char first_c_source[2] = {source[0] ,source[1]};
        char first_c_dest[2] = {destination[0] ,destination[1]};
        //the path must not cross a spot which is directly connected to an
        //enemy occupied place with a distance smaller than 5 units of distance
        if((first_c_dest[0] == 'E') && !(first_c_source[0] == 'E'))
            if(weight<5)
                restrictedSpots.push_back(source);
        if(!(first_c_dest[0] == 'E') && (first_c_source[0] == 'E'))
            if(weight<5)
                restrictedSpots.push_back(destination);

        set_of_nodes.push_back(source);
        set_of_nodes.push_back(destination);
    }
    vector<string> letters; // holds set of nodes
    //@start: loop for get set of nodes vector
    for (string key : set_of_nodes) {
        if (!count(letters.begin(), letters.end(), key)) {
            letters.push_back(key);
            vertices_counter ++;
        }
    }//@end loop
    vector<pair<string,int>> pairs; //holds nodes with their unique id's
    for(int i=0; i<int(letters.size()); i++){
        pairs.push_back(make_pair(letters[i], i));
    }
    
    Graph graph(vertices_counter); //create object with number of vertices=vertice_counter
    ifstream city_plan2(fname);
    //@start: loop for adding edges into object
    while(getline(city_plan2, line)){
        
        stringstream ss(line);
        string source;
        string destination;
        int weight;
        //write values inside source and destination
        getline(ss, source, ',');
        getline(ss, destination, ',');
        // Read the integers using the operator >>
        ss >> weight;
        char first_c_source[2] = {source[0] ,source[1]};
        char first_c_dest[2] = {destination[0] ,destination[1]};
        //the path must not cross a spot which is directly connected to an
        //enemy occupied place with a distance smaller than 5 units of distance
        if (count(restrictedSpots.begin(), restrictedSpots.end(), source))
            continue;
        if (count(restrictedSpots.begin(), restrictedSpots.end(), destination))
            continue;
            
//        the path must not cross a spot wtich is occupied by enemies
        if(!(first_c_dest[0] == 'E')&& !(first_c_source[0] == 'E')){
                graph.addEdge(make_pair(source, get_val(source, pairs)), make_pair(destination, get_val(destination, pairs)), weight);
        }


    }
    //@end looping
    //define start and stop vertex
    string startVertex = "Ma";
    string stopVertex = "Mo";
    // find and print shortest path
    graph.shortestPath(get_val(startVertex, pairs), get_val(stopVertex, pairs), pairs);

    return 0;
    
}
