#ifndef RANDOM_H
#define RANDOM_H

#include <bits/stdc++.h>
using namespace std;

extern mt19937 rng;
extern uniform_real_distribution<float> rng_real;

int rand_range(int l, int r);

#endif