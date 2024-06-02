#include <bits/stdc++.h>
#include "ant.h"

using namespace std;

void Ant::add(int salesman, int vertex){
    tours[salesman].cost += (*distance)[tours[salesman].back()][vertex];
    tours[salesman].push_back(vertex);
}

void Ant::add(_tour &tour, int vertex){
    tour.cost += (*distance)[tour.back()][vertex];
    tour.push_back(vertex);
}

void Ant::end_tour(){
    min_max_cost = min_sum_cost = 0;

    for(auto &tour: tours){
        add(tour, 0);
        min_max_cost = max(min_max_cost, tour.cost);
        min_sum_cost += tour.cost;
    }
}