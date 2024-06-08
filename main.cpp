#include <bits/stdc++.h>
#include <omp.h>    
#include "solver.h"
#include "graph.h"
#include "parameters.h"
#include "stat.h"

using namespace std;

int main(int argc, char **args){
    ios_base::sync_with_stdio(0); cin.tie(0);
    omp_set_num_threads(NUM_THREADS);

    string dataset = string(args[1]);
    int salesmen = stoi(args[2]);

    string variation;

    if(argc == 4){
        variation = string(args[3]);
    }

    system("mkdir convergence");
    system("mkdir result");

    Graph graph;
    graph.load_data(dataset);
    init(variation, graph, salesmen);

    for(int i = 0; i < RUN; i++){
        mTSPSolver solver(graph, salesmen);
        solver.solve();
        write_result(solver.gbest, variation, graph, salesmen, i + 1);
    }

    write_convergence(variation, graph, salesmen);
    write_result_complete(variation, graph, salesmen);
}