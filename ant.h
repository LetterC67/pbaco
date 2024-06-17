#ifndef ANT_H
#define ANT_H

#include <bits/stdc++.h>
#include "parameters.h"
#include "graph.h"
using namespace std;

struct _tour{
    vector<int> tour;
    float cost;

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
    vector<vector<float>> *distance;
    Graph *graph;
    vector<_tour> tours;
    vector<int> assigned, position;
    unordered_set<int> del;
    float min_max_cost;
    float min_sum_cost;
    float sqrt_cost;

    Ant(){
        min_max_cost = 1e9;
        min_sum_cost = 1e9;
    }


    Ant(int salesmen, vector<vector<float>> *distance, Graph *graph): graph(graph), distance(distance){
        tours = vector<_tour>(salesmen);
        for(int i = 0; i < salesmen; i++) del.insert(i);
        assigned = vector<int>(graph -> n);
        position = vector<int>(graph -> n);
    }
    
    void retag(int index);
    int longest_tour_index();
    int shortest_tour_index();
    float tour_length(_tour &tour);
    float tour_distance(_tour &a, _tour &b);

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