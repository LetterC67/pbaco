#include <bits/stdc++.h>
#include "ant.h"
#include "random.h"
#include "parameters.h"
#include "population.h"

double Population::ant_distance(Ant &a, unordered_set<int> &edge){
    double cnt = 0;
    for(auto &v : a.tours){
        for(int i = 0; i < v.size() - 1; i++){
            if(edge.count(v[i] * n + v[i + 1])) cnt += 1.;
        }
    }

    return cnt / double(n + salesmen - 1);
}

void Population::kill(){
    vector<double> fitness(population.size());
    vector<int> index;

    for(int i = 0; i < population.size(); i++){
        double mx = 0.;
        index.push_back(i);
        unordered_set<int> edge;
        
        for(auto &v : population[i].tours){
            for(int i = 0; i < v.size() - 1; i++){
                edge.insert(v[i] * n + v[i + 1]);
            }
        }
        vector<double> v;
        for(int j = population.size() - 1; j >= 0; j--){
            v.push_back(ant_distance(population[j],edge));
        }
        sort(v.begin(), v.end(), greater<double>());

        fitness[i] = population[i].min_max_cost * powl(POP_ARGS, (v[0] + v[1]) / 2);
    }

    sort(index.begin(), index.end(), [&](const int &a, const int &b){
        return fitness[a] < fitness[b];
    });

    vector<Ant> temp;

    for(int i = 0; i < min((int)population.size(), MIN_POPULATION_SIZE); i++){
        temp.push_back(population[index[i]]);
    }

    population = temp;
}

void Population::add(Ant &ant){
    population.push_back(ant);
    if(population.size() > MAX_POPULATION_SIZE)
        kill();
}

Ant Population::get(){
    return population[rng() % population.size()];
}