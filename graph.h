#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>
using namespace std;

struct Graph{
    int n;
    string EDGE_WEIGHT_TYPE, NAME;
    vector<vector<float>> distance;
    vector<vector<int>> closest;
    vector<float> x;
    vector<float> y;

    void init_closest();
    float calculate_distance(int i, int j);
    void load_data(const string &file);
};

#endif