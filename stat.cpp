#include <bits/stdc++.h>
#include "stat.h"
#include "graph.h"
using namespace std;

float worse, best = 1e18, avg;
int cnt;
vector<float> convergence;
vector<int> ccnt;

void init(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::out);
}

void write_result(Ant &gbest, string &variation, Graph &graph, int salesmen, int run){
    cnt++;
    worse = max(worse, gbest.min_max_cost);
    best = min(best, gbest.min_max_cost);
    avg += gbest.min_max_cost;

    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::app);
    
    outputFile <<"Run " << run << ' ' << gbest.min_max_cost << endl;
    for(auto &tour : gbest.tours){
        for(int d : tour) outputFile << d << ' ';
        outputFile << endl;
    }
    for(auto &tour : gbest.tours){
        outputFile << tour.cost << ' ';
    }
    outputFile << endl;
    outputFile << endl;

    outputFile.close();
}

void add(int iter, float cost){
    if(iter >= convergence.size()){
        convergence.push_back(0.);
        ccnt.push_back(0);
    }
    convergence[iter] += cost;
    ccnt[iter]++;
}

void write_convergence(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("convergence/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::out);

    for(int i = 0; i < convergence.size(); i++){
        outputFile << i + 1 << ' ' << convergence[i] / float(ccnt[i]) << endl;
    }

    outputFile.close();
}

void write_result_complete(string &variation, Graph &graph, int salesmen){
    std::ofstream outputFile("result/" + graph.NAME + "_" + to_string(salesmen) + "_" + variation, std::ios::app);
    
    outputFile <<"Avg: " << avg / float(cnt) << endl;
    outputFile <<"Best: " << best << endl;
    outputFile <<"Worse: " << worse << endl;
}