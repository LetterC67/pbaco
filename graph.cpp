#include <bits/stdc++.h>
#include "graph.h"
using namespace std;


float Graph::calculate_distance(int i, int j) {
    if(EDGE_WEIGHT_TYPE == "ATT"){
        return (int)ceil(sqrt(((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j])) / 10.));
    }else{
        return sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
    }
}

void Graph::load_data(const string &file){
    ifstream ifs(file, ifstream::in);
    string nil;

    ifs >> nil; 
    if(nil.back() != ':') ifs >> nil;


    ifs >> NAME;

    ifs.ignore();

    getline(ifs, nil);
    getline(ifs, nil);
    

    ifs >> nil; 
    if(nil.back() != ':') ifs >> nil;
    ifs >> n;
    ifs >> nil; if(nil.back() != ':') ifs >> nil;
    ifs >> EDGE_WEIGHT_TYPE;

    ifs >> nil;

    x.resize(n);
    y.resize(n);
    
    for(int i = 0; i < n; i++){
        int temp;
        ifs >> temp;
        ifs >> x[i] >> y[i];
    }

    distance.resize(n);
    for(auto &d : distance) d.resize(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            distance[i][j] = calculate_distance(i, j);
        }
    }

    cout << "Loaded data from " << file << endl;
}