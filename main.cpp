#include <bits/stdc++.h>
#include <omp.h>    
#include "solver.h"
#include "graph.h"
#include "parameters.h"

using namespace std;

int main(int argc, char **args){
    ios_base::sync_with_stdio(0); cin.tie(0);
    omp_set_num_threads(NUM_THREADS);

    string dataset = string(args[1]);
    int salesmen = stoi(args[2]);

    Graph graph;
    graph.load_data(dataset);

    mTSPSolver solver(graph, salesmen);
    solver.solve();
}