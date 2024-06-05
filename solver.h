#ifndef SOLVER_H
#define SOLVER_H

#include <bits/stdc++.h>
#include "graph.h"
#include "parameters.h"
#include "ant.h"
#include "population.h"

using namespace std;


struct mTSPSolver{
    Graph graph;
    int n;
    int salesmen;
    int iteration = 1;
    double TAU_MIN;

    Ant gbest;
    Population population;
    vector<vector<double>> pheromone;


    mTSPSolver(Graph graph, int salesmen): graph(graph), salesmen(salesmen){
        n = graph.n;
        TAU_MIN = 1 / (double(n * n) / RATIO);
        pheromone.resize(n);
        for(auto &p : pheromone)
            p = vector<double>(n, TAU_MAX);
        population = Population(n, salesmen);
    }

    void solve();
    void update_pheromone();
    vector<Ant> build_solutions();
    Ant build_solution(Ant ant);
    pair<int, int> select_city(Ant &ant, vector<bool> &visited, vector<long double> &sum, vector<vector<pair<int, long double>>> &bucket);
};

#endif