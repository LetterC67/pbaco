#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include "random.h"
using namespace std;

struct RouletteWheel{
    vector<double> probabilities;
    double sum = 0;
    double mx = 0;
    int pos = -1;

    void add(double probability);
    int spin();
};

#endif