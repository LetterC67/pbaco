#include <bits/stdc++.h>
#include "utils.h"
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