#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>
using namespace std;

struct Graph{
    int n;
    string EDGE_WEIGHT_TYPE, NAME;
    vector<vector<float>> distance;
    vector<float> x;
    vector<float> y;

    float calculate_distance(int i, int j);
    void load_data(const string &file);
};

#endif