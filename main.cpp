#include "bits/stdc++.h"
using namespace std;
const long double INF = 1e18;
const int MAXN = 1e5 + 5;
int N, M; // Number of nodes, Number of edges
vector <int> edges[MAXN];
vector <double> cost[MAXN];
int indegree[MAXN];
int multi_visited[MAXN];
double dist[MAXN];
int shortest_path_tree_parent[MAXN];
set < pair < int, int > > S;
vector <int> multi;

// This function is responsible for reading in the graph.
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
            case 'c':
            case '\n':
            case 'n':
            case '\0':
                break;
            case 'p':
            case 'a': {
                int tail; int head;
                double weight;
                sscanf(line.c_str(), "%*c %d %d %lf", &tail, &head, &weight);

                // Clip Dummy edges that won't appear in any shortest path
                if((tail == 1) || (head == N) || (tail % 2 == 0) || (weight <= cost[1][(head/2)-1] + cost[tail][0])){
                    edges[tail].push_back(head);
                    cost[tail].push_back(weight);
                    indegree[head]++;
                }
                break;
            }
            default: break;
        }
    }
}

// Computes Topological ordering of given graph and stores the order in vector topological_ordering(defined globally).
vector <int> topological_ordering;
void toposort(){
    int tmp_indegree[N + 1];
    int put[N + 1];
    for(int i = 1; i <= N; ++i){
        tmp_indegree[i] = indegree[i];
        put[i] = 1;
    }
    topological_ordering.clear();
    for(int i = 1; i <= N; ++i){
        S.insert({tmp_indegree[i], i});

    }
    while(S.empty() == false){
        int node = S.begin() -> second;
        S.erase(S.begin());
        put[node] = 0;
        for(int j = 0; j < edges[node].size(); ++j){
            if(put[edges[node][j]] == 0)
                continue;
            tmp_indegree[edges[node][j]]--;
            S.erase(S.find({tmp_indegree[edges[node][j]] + 1, edges[node][j]}));
            S.insert({tmp_indegree[edges[node][j]], edges[node][j]});
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
            double updated_dist = dist[node] + cost[node][j];
            if(dist[edges[node][j]] - updated_dist > 0.0000001){
                dist[edges[node][j]] = updated_dist;
                shortest_path_tree_parent[edges[node][j]] = node;
            }
        }
    }
}

// This function transforms the edge weights to positive so that Djikstra's Algorithm can be applied
void update_allgraph_weights()
{
    for(int i = 1; i <= N; ++i){
        for(int j = 0; j < edges[i].size(); ++j){
            cost[i][j] = cost[i][j] + dist[i] - dist[edges[i][j]];
        }
    }
    for(int i = 1; i <= N; ++i)
        dist[i] = 0;
}

// Finds multi paths
void find_multi_path(){
    // Initialize
    multimap < double, int > scores;
    multi.clear();

    // Candidate nodes
    for(int i = 3; i < N; i+=2){
        if((edges[i].size() != 0) && (edges[i][0] == N)){
            scores.insert({dist[i] + cost[i][0], i});
        }
    }

    // Check if path is mutually independent
    while(scores.empty() == false){
        int node = (scores.begin())->second;
        int curr = node;
        while((curr != 1) && (multi_visited[curr] == 0)){
            multi_visited[curr] = 1;
            curr = shortest_path_tree_parent[curr];
        }
        if(curr == 1){
            multi.push_back(node);
        }
        else{
            break;
        }
    }

    // Clear multi_visited
    for(int i = 1; i <= N; ++i){
        multi_visited[i] = 0;
    }
}

// This function flips edges along the multiple shortest paths.
void flip_path()
{
    for(int i = 0; i < multi.size(); ++i){
        int pre = N;
        int curr = multi[i];
        while(pre != 1){

            // Find edge index
            vector <int> :: iterator it = find(edges[curr].begin(), edges[curr].end(), pre);
            int idx = it - edges[curr].begin();

            // Erase edges
            long double c = cost[curr][idx];
            edges[curr].erase(edges[curr].begin() + idx);
            cost[curr].erase(cost[curr].begin() + idx);
            indegree[pre]--;

            // Add reverse edge. Permanent edge clipping: Don't add reverse edges from sink/to source
            if(curr != 1 && pre != N){
                edges[pre].push_back(curr);
                cost[pre].push_back(-c);
                indegree[curr]++;
            }

            // Update
            pre = curr;
            curr = shortest_path_tree_parent[curr];
        }
    }
}

void updateShortestPathTree()
{
    for(int i = 1; i <= N; ++i)
        dist[i] = INF;
    dist[1] = 0;
    shortest_path_tree_parent[1] = -1;
    multimap < double, int > K;
    K.insert({0, 1});
    while(K.empty() == false){
        int node = (K.begin())->second;
        double c = (K.begin())->first;
        if(node == N)
            break;
        K.erase(K.begin());
        for(int j = 0; j < edges[node].size(); ++j){
            long double upd_dist = dist[node] + cost[node][j];
            if(-0.0000001 > upd_dist - dist[edges[node][j]]){
                dist[edges[node][j]] = upd_dist;
                shortest_path_tree_parent[edges[node][j]] = node;
                K.insert({dist[edges[node][j]], edges[node][j]});
            }
        }
    }
    for(int i = 1; i <= N; ++i){
        if(dist[i] > dist[N])
            dist[i] = dist[N];
    }
}

int main(int argc, char * argv[])
{
    char* in_file = argv[2];
    initGraph(in_file);
    init_shortest_path_tree();
    double total_cost = dist[N];
    update_allgraph_weights();
    find_multi_path();
    flip_path();
    double curr_cost = total_cost;
    int c = 1;
    int flag = 0;
    while(true)
    {
        updateShortestPathTree();
        find_multi_path();
        for(int i = 0; i < multi.size(); ++i){
            int node = multi[i];
            curr_cost += dist[node] + cost[node][0];
            total_cost += curr_cost;
            cout << "Iteration " << (c++) << ": "<<  curr_cost << endl;
            if (curr_cost > -0.0000001) {
                flag = 1;
                break;
            }
        }
        if(flag == 1){
            break;
        }
        update_allgraph_weights();
        flip_path();
    }
    cout << total_cost << endl;
    return 0;
}