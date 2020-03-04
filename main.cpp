#include "bits/stdc++.h"
using namespace std;
const long double INF = 1e18;
const int MAXN = 1e5 + 5;
int N, M; // Number of nodes, Number of edges
vector <int> edges[MAXN];
vector <long double> cost[MAXN];
vector <long double> reduced_weight[MAXN];
int indegree[MAXN];
long double dist[MAXN];
int shortest_path_tree_parent[MAXN];
set < pair < int, int > > S;

/*
This function is responsible for reading in the graph.
*/
void initGraph(string filename)
{
    char pr_type[3]; //problem type;
    ifstream file(filename);
    string line_inf;
    getline(file, line_inf);
    sscanf(line_inf.c_str(), "%*c %3s %d %d", pr_type, &N, &M);
    for(string line; getline(file, line); )
    {
        switch(line[0]){
            case 'c':                  /* skip lines with comments */
            case '\n':                 /* skip empty lines   */
            case 'n':
            case '\0':                 /* skip empty lines at the end of file */
                break;
            case 'p':
            case 'a': {
                int tail; int head;
                double weight;
                sscanf(line.c_str(), "%*c %d %d %lf", &tail, &head, &weight);
                edges[tail].push_back(head);
                cost[tail].push_back(weight);
                indegree[head]++;
                break;
            }
            default:
                break;
        }
    }
}

// Computes Topological ordering of given graph and stores the order in vector topological_ordering(defined globally).
vector <int> topological_ordering;
void toposort(){
    for(int i = 1; i <= N; ++i){
        S.insert({indegree[i], i});
    }
    while(S.empty() == false){
        int node = S.begin() -> second;
        S.erase(S.begin());
        for(int j = 0; j < edges[node].size(); ++j){
            indegree[edges[node][j]]--;
            S.erase(S.find({indegree[edges[node][j]] + 1, edges[node][j]}));
            S.insert({indegree[edges[node][j]], edges[node][j]});
        }
        topological_ordering.push_back(node);
    }
}

// Construct the initial Shortest Path tree
void init_shortest_path_tree()
{
    toposort();
    int l = topological_ordering.size();
    for(int i = 1; i <= N; ++i)
        dist[i] = INF;
    dist[topological_ordering[0]] = 0;
    shortest_path_tree_parent[topological_ordering[0]] = -1; // Source
    for(int i = 0; i < l; ++i){
        int node = topological_ordering[i];
        for(int j = 0; j < edges[node].size(); ++j){
            long double updated_dist = dist[node] + cost[node][j];
            if(updated_dist < dist[edges[node][j]]){
                dist[edges[node][j]] = updated_dist;
                shortest_path_tree_parent[edges[node][j]] = node;
            }
        }
    }
}

int main(int argc, char * argv[])
{
    char* in_file = argv[2];
    initGraph(in_file);
    init_shortest_path_tree();
    return 0;
}