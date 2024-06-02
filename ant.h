#ifndef ANT_H
#define ANT_H

#include <bits/stdc++.h>
#include "parameters.h"
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
    vector<_tour> tours;
    double min_max_cost;
    double min_sum_cost;

    Ant(){
        min_max_cost = 1e9;
        min_sum_cost = 1e9;
    }


    Ant(int salesmen, vector<vector<double>> *distance): distance(distance){
        tours = vector<_tour>(salesmen);
    }
    
    void add(int salesman, int vertex);
    void add(_tour &tour, int vertex);
    void end_tour();
};

#endif