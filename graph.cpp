#include <bits/stdc++.h>
#include "graph.h"
#include "parameters.h"
using namespace std;

struct OneMinimumSpanningTree {
    int sourceNode;
    int numNodes;
    vector<int> par;
    vector<float> d;
    vector<int> topo;
    vector<bool> visited;
    vector<vector<float>> distance;
    set<pair<int, int>> edges;
    int chooseRate;

    /*
     * Implement Prim Algorithm to find the minimum spanning tree without the source node
     * Find the two nodes that have the minimum distance to source node -> A 1-Tree Minimum Spanning Tree is found.
     * For all edge (i, j), find beta(i, j) and compute alpha(i, j) = beta(i, j) - dis(i, j)
     * The function will return for each node the 0.1 * numNodes closest to it base on alpha
    */

    OneMinimumSpanningTree(int _sourceNode, int _numNodes, int _chooseRate, vector<vector<float>> distance): 
        sourceNode(_sourceNode), 
        numNodes(_numNodes),
        d(_numNodes),
        par(_numNodes),
        visited(_numNodes),
        chooseRate(_chooseRate),
        distance(distance) {}

    float primAlgorithm() {
        // Remember to exclude the source node

        for (int i = 0; i < numNodes; i++) {
            d[i] = 2e9;
            par[i] = i;
        }

        float totalWeight = 0;
        int preNode = -1;

        d[sourceNode + 1] = 0;
        
        for (int iter = 1; iter <= numNodes - 1; iter++) {
            int bestNode;
            float Min = 2e9 + 1;

            for (int node = 1; node < numNodes; node++){
                if (!visited[node] && d[node] < Min){
                    Min = d[node];
                    bestNode = node;
                }
            }

            totalWeight += d[bestNode];
            topo.push_back(bestNode);
            if (preNode != -1){
                par[bestNode] = preNode;
                edges.insert({preNode, bestNode});
            }
            visited[bestNode] = true;
            preNode = bestNode;

            for (int node = 1; node < numNodes; node++){
                if (!visited[node] && d[node] > distance[bestNode][node]) {
                    d[node] = distance[bestNode][node];
                }
            }
        }
        return totalWeight;
    }

    vector<vector<int>> computeCandidateSet() {
        float totalWeight = primAlgorithm();
        vector<vector<int>> candidateSet(numNodes);

        // find the best two
        vector<pair<float, int>> c;
        for (int node = 1; node < numNodes; node++) {
            c.push_back({distance[0][node], node});
        }
        sort(c.begin(), c.end());
        edges.insert({sourceNode, c[0].second});
        edges.insert({sourceNode, c[1].second});

        vector<pair<float, int>> candidates;

        for (int i = 1; i < numNodes; i++) {
            candidates.push_back({distance[i][sourceNode] - c[1].second, i});
        }

        sort(candidates.begin(), candidates.end());
        vector<int> candidate;

        for (int j = 0; j < numNodes * chooseRate; j++){
            candidate.push_back(candidates[j].second);
        }
        candidateSet.push_back(candidate);

        candidates.clear();
        candidate.clear();

        vector<int> marked(numNodes);
        vector<float> b(numNodes);
        for (int i = 1; i < numNodes; i++){
            marked[i] = 0;
        }

        for (int node = 1; node < numNodes; node++) {
            b[node] = -2e9;
            for (int k = node; k != 1; k = par[k]) {
                b[par[k]] = max(b[k], distance[k][par[k]]);
                marked[k] = node;
            }

            for (auto &v : topo) {
                if (v != node) {
                    if (marked[v] != node) {
                        b[v] = max(b[par[v]], distance[v][par[v]]);
                    }
                }
            }
            for (int v = 1; v < numNodes; v++) {
                candidates.push_back({distance[node][v] - b[node], node});
            }
            sort(candidates.begin(), candidates.end());
            for (int j = 0; j < numNodes* chooseRate; j++){
                candidate.push_back(candidates[j].second);
            }
            candidateSet.push_back(candidate);
            candidate.clear();
            candidates.clear();
        }
        return candidateSet;
    }
};

float Graph::calculate_distance(int i, int j) {
    if(EDGE_WEIGHT_TYPE == "ATT"){
        return (int)ceil(sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.));
    }else{
        return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
    }
}

void Graph::init_closest(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j && j != DEPOT) closest[i].push_back(j);
        }
        sort(closest[i].begin(), closest[i].end(), [&](const int &a, const int &b){
            return distance[i][a] < distance[i][b];
        });

        while(closest[i].size() > min(MAX_CLOSEST_COUNT, max(MIN_CLOSEST_COUNT, int(n * CLOSEST_RATE)))) closest[i].pop_back();
        s_closest[i] = unordered_set<int>(closest[i].begin(), closest[i].end());
    }

    // OneMinimumSpanningTree one_tree(0, n, CLOSEST_RATE, distance);
    // closest = one_tree.computeCandidateSet();
}

void Graph::load_data(const string &file){
    ifstream ifs(file, ifstream::in);
    string nil;

    ifs >> nil; 
    if(nil.back() != ':') ifs >> nil;


    ifs >> NAME;

    ifs.ignore();

    getline(ifs, nil);
    getline(ifs, nil);
    

    ifs >> nil; 
    if(nil.back() != ':') ifs >> nil;
    ifs >> n;
    ifs >> nil; if(nil.back() != ':') ifs >> nil;
    ifs >> EDGE_WEIGHT_TYPE;

    ifs >> nil;

    closest = vector<vector<int>>(n);
    s_closest = vector<unordered_set<int>>(n);

    x.resize(n);
    y.resize(n);
    
    for(int i = 0; i < n; i++){
        int temp;
        ifs >> temp;
        ifs >> x[i] >> y[i];
    }

    distance.resize(n);
    for(auto &d : distance) d.resize(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            distance[i][j] = calculate_distance(i, j);
        }
    }

    init_closest();

    cout << "Loaded data from " << file << endl;
}