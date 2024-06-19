#ifndef POPULATION_H
#define POPULATION_H

#include <bits/stdc++.h>
#include "ant.h"
#include "random.h"
#include "parameters.h"

using namespace std;

struct Population{
    vector<Ant> population;
    int n, salesmen;

    Population(){}

    Population(int n, int salesmen): n(n), salesmen(salesmen) {};

    double ant_distance(Ant &a, unordered_set<int> &edge);

    void kill();

    void add(Ant &ant);

    Ant get();
};

#endif 