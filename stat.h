#ifndef STAT_H
#define STAT_H

#include <bits/stdc++.h>
#include "ant.h"
#include "graph.h"

using namespace std;

void init(string &variation, Graph &graph, int salesmen);
void write_result(Ant &gbest, string &variation, Graph &graph, int salesmen, int run);
void add(int iter, double cost);
void write_convergence(string &variation, Graph &graph, int salesmen);
#endif