#include <bits/stdc++.h>
#include "utils.h"
#include "ant.h"

using namespace std;

void RouletteWheel::add(double probability){
    probabilities.push_back(probability);
    sum += probability;
    if(mx < probability){
        mx = probability;
        pos = probabilities.size() - 1;
    }
}

int RouletteWheel::spin(){
    double threshold = rng_real(rng);
    double accumulation = 0.;

    for(int i = 0; i < probabilities.size(); i++){
        accumulation += probabilities[i];
        if(accumulation >= threshold * sum){
            return i;
        }
    }

    return probabilities.size() - 1;
}

double Ant::tour_length(_tour &tour){
    double cost = 0;
    for(int i = 0; i < tour.size() - 1; i++){
        cost += (*distance)[tour[i]][tour[i + 1]];
    }
    return cost;
}

int Ant::longest_tour_index(){
    return max_element(tours.begin(), tours.end()) - tours.begin();
}

int Ant::shortest_tour_index(){
    return min_element(tours.begin(), tours.end()) - tours.begin();
}