#include <bits/stdc++.h>
#include "graph.h"
#include "solver.h"
#include "ant.h"
#include "parameters.h"
#include "utils.h"

using namespace std;

pair<int, int> mTSPSolver::select_city(Ant &ant, vector<bool> &visited, vector<long double> &sum, vector<vector<pair<int, long double>>> &bucket){
    RouletteWheel wheel;

    for(int salesman = 0; salesman < salesmen; salesman++){
        if(!bucket[salesman].size()){
            sum[salesman] = 0;
            int current_city = ant.tours[salesman].back();
            for(int city = 0; city < n; city++){
                if(!visited[city]){
                    long double prob = pheromone[current_city][city] * 1. / powl(graph.distance[current_city][city] * max(1., ant.tours[salesman].cost), BETA);
                    //cout << pheromone[current_city][city] << endl;
                    bucket[salesman].push_back({city, prob});
                    sum[salesman] += prob;
                }
            }
            //cout << "new " << ' ' << current_city << ' ' << sum[salesman] << ' ' << ant.tours[salesman].cost<< endl;
        }
        wheel.add(sum[salesman]);
    }

    int sm = wheel.spin();
    long double bound = rng_real(rng) * sum[sm];
    long double accum = 0.;

   // cout << accum << ' ' << bound << ' ' << sum[sm] << ' '  << sm <<  endl;
    for(auto &p : bucket[sm]){
        if(visited[p.first]) continue;
        accum += p.second;
        if(accum > bound){
            int city = p.first;
            for(int salesman = 0; salesman < salesmen; salesman++){
                int current_city = ant.tours[salesman].back();
                long double prob = pheromone[current_city][city] * 1. / powl(graph.distance[current_city][city] * max(1., ant.tours[salesman].cost), BETA);
                sum[salesman] -= prob;
             //   cout << sum[salesman] << ' ' << salesman << ' ' << prob << endl;
            }
            bucket[sm].clear();
            return {sm, city};
        }
    }


  //  cout << accum << ' ' << bound << ' ' << sum[sm] << ' ' << (accum > bound) << endl;
    
    return {-1, -1};
}

Ant mTSPSolver::build_solution(Ant ant){
    vector<bool> visited(n);
    vector<long double> sum(salesmen);
    vector<vector<pair<int, long double>>> bucket(salesmen);

    

    for(auto &tour : ant.tours){
        for(auto &vertex : tour){
            visited[vertex] = true;
        }
    }
    visited[DEPOT] = true;

    int not_visited = count(visited.begin(), visited.end(), 0);

    while(not_visited--){
        //cout << not_visited << endl;
        auto next = select_city(ant, visited, sum, bucket);
        ant.add(next.first, next.second);
       // cout << next.first << ' ' << next.second << endl;
        visited[next.second] = true;
    }

    ant.end_tour();

    return ant;
}

vector<Ant> mTSPSolver::build_solutions(){
    vector<Ant> ants;
    const int ANTS = ANTS_MULTIPLIER * n;

    #pragma omp parallel for
    for(int ant = 0; ant < ANTS; ant++){
        #pragma omp critical
        ants.push_back(build_solution(Ant(salesmen, &graph.distance)));
    }

    return ants;
}

void mTSPSolver::update_pheromone(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            pheromone[i][j] = pheromone[i][j] * (1 - RHO) + TAU_MIN * RHO;
        }
    }

    for(auto &tour : gbest.tours){
        for(int i = 0; i < tour.size() - 1; i++){
            pheromone[tour[i]][tour[i + 1]] = pheromone[tour[i]][tour[i + 1]] * (1 - RHO) - TAU_MIN * RHO + TAU_MAX * RHO;
            ///assert(pheromone[tour[i]][tour[i + 1]]  <= 1);
        }
    }
}

void mTSPSolver::solve(){
    for(int iteration = 1; iteration <= ITERATIONS; iteration++){
        auto ants = build_solutions();

        for(auto &ant : ants){
            if(gbest.min_max_cost - 1e-6 > ant.min_max_cost || (abs(gbest.min_max_cost - ant.min_max_cost) < 1e-6 && gbest.min_sum_cost - 1e-6 > ant.min_sum_cost)){
                gbest = ant;
            }
        }

        update_pheromone();

        cout << "Iteration " << iteration << ' ' << gbest.min_max_cost << ' ' << gbest.min_sum_cost << endl;
    }
}
