#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include "random.h"
using namespace std;

struct RouletteWheel{
    vector<long double> probabilities;
    long double sum = 0;
    long double mx = 0;
    int pos = -1;

    void add(long double probability);
    int spin();
};

#endif