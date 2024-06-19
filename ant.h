#ifndef ANT_H
#define ANT_H

#include <bits/stdc++.h>
#include "parameters.h"
#include "graph.h"
using namespace std;

struct _tour{
    vector<int> tour;
    double cost;

    _tour(){
        cost = 0.;
        tour = {DEPOT};
    }

    int& operator[](int index) {
        return tour[index];
    }

    const int& operator[](int index) const {
        return tour[index];
    }
    
    auto begin() {
        return tour.begin();
    }

    auto end() {
        return tour.end();
    }

    void push_back(int u){
        tour.push_back(u);
    }

    int back(){
        return tour.back();
    }

    int size(){
        return tour.size();
    }
};

struct Ant{      
    vector<vector<double>> *distance;
    Graph *graph;
    vector<_tour> tours;
    vector<int> assigned, position;
    unordered_set<int> del;
    double min_max_cost;
    double min_sum_cost;
    double sqrt_cost;

    Ant(){
        min_max_cost = 1e9;
        min_sum_cost = 1e9;
    }

    Ant(int salesmen, vector<vector<double>> *distance, Graph *graph): graph(graph), distance(distance){
        tours = vector<_tour>(salesmen);
        for(int i = 0; i < salesmen; i++) del.insert(i);
        assigned = vector<int>(graph -> n);
        position = vector<int>(graph -> n);
    }
    
    ~Ant(){
        delete graph;
        delete distance;
    }

    void retag(int index);
    int longest_tour_index();
    int shortest_tour_index();
    double tour_length(_tour &tour);
    double tour_distance(_tour &a, _tour &b);

    void add(int salesman, int vertex);
    void add(_tour &tour, int vertex);
    
    bool swap(_tour &a, _tour &b, int idx_b);
    bool relocate(_tour &a, _tour &b, int idx_a, int idx_b);
    bool swap_tail(_tour &a, _tour &b, int idx_a, int idx_b);

    bool two_opt_sweepline(_tour &tour, int idx);
    void two_opt(_tour &tour);
    void or_opt(_tour &tour, int idx);
    void or_opt(_tour &tour);
    void run_dp(_tour &x, _tour &y);
    void run_tsp();
    void dp();
    void verify(int n);

    void intra_tour_optimization();
    void inter_tour_optimization();
    void inter_tour_optimization_del();
    void local_search();
    void end_tour();
    void calculate_result();
};

Ant trim(Ant ant);

#endif