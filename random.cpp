#include "random.h"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_real_distribution<float> rng_real(0.0, 1.0);

int rand_range(int l, int r){
    return l + rng() % (r - l + 1);
}