#include <bits/stdc++.h>
#include <chrono>
#include "graph.h"
#include "solver.h"
#include "ant.h"
#include "parameters.h"
#include "utils.h"
#include "stat.h"

using namespace std;

inline double powl(double x, int y){
    double res = 1;
    while(y--) res *= x;
    return res;
}

pair<int, int> mTSPSolver::select_city(Ant &ant, vector<bool> &visited){
    RouletteWheel wheel;
    int salesman = ant.shortest_tour_index();
    int current_city = ant.tours[salesman].back();

    vector<int> candidate;
    for(int city = 0; city < n; city++){
        if(!visited[city]){
            double prob = pheromone[current_city][city] * 1. / powl(graph.distance[current_city][city], BETA);
            wheel.add(prob);
            candidate.push_back(city);
        }
    }

    assert(candidate.size());

    return {salesman, candidate[wheel.spin()]};
}

Ant mTSPSolver::build_solution(Ant ant){
    vector<bool> visited(n);

    for(auto &tour : ant.tours){
        for(auto &vertex : tour){
            visited[vertex] = true;
        }
    }
    visited[DEPOT] = true;

    int not_visited = count(visited.begin(), visited.end(), 0);

    while(not_visited--){
        auto next = select_city(ant, visited);
        ant.add(next.first, next.second);
        visited[next.second] = true;
    }

    ant.end_tour();
    ant.local_search();
    ant.calculate_result();

    return ant;
}

vector<Ant> mTSPSolver::build_solutions(){
    vector<Ant> ants;
    //const int ANTS = max(MIN_ANT, (int)(ANTS_MULTIPLIER * n));

    #pragma omp parallel for
    for(int ant = 0; ant < ANTS; ant++){
        Ant a;
        if(!population.population.size())
            a = build_solution(Ant(salesmen, &graph.distance, &graph));
        else
            a = build_solution(trim(population.get()));

        #pragma omp critical
        {
            ants.push_back(a);
        }
    }

    return ants;
}

void mTSPSolver::update_pheromone(){
    auto p = pheromone;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            pheromone[i][j] = pheromone[i][j] * (1 - RHO) + TAU_MIN * RHO;
        }
    }

    for(auto &tour : gbest.tours){
        for(int i = 0; i < tour.size() - 1; i++){
            pheromone[tour[i]][tour[i + 1]] = p[tour[i]][tour[i + 1]] * (1 - RHO) + TAU_MAX * RHO;
        }
    }
}

void mTSPSolver::solve(){
    auto start_time = chrono::high_resolution_clock::now();

    
    for(; iteration <= cutoff_iteration; iteration++){
        auto start_iteration = chrono::high_resolution_clock::now();


        auto ants = build_solutions();
        Ant ibest;


        for(auto &ant : ants){
            population.add(ant);
            if(gbest.min_max_cost - 1e-4 > ant.min_max_cost || (abs(gbest.min_max_cost - ant.min_max_cost) < 1e-4 && gbest.sqrt_cost - 1e-4 > ant.sqrt_cost)){
                ant.run_tsp();
                gbest = ant;
                no_improve = 0;
            }

            if(ibest.min_max_cost - 1e-4 > ant.min_max_cost || (abs(ibest.min_max_cost - ant.min_max_cost) < 1e-4 && ibest.sqrt_cost - 1e-4 > ant.sqrt_cost)){
                ibest = ant;
            }
        }
        no_improve++;

        update_pheromone();

        population.kill();
        if(no_improve > MAX_STAGNATION){
            
        }

        add(iteration - 1, gbest.min_max_cost);

        // if(iteration % 10 == 0){
        //     for(auto &tour : gbest.tours){
        //         for(int &d : tour){
        //             cout << d << ' ';
        //         }
        //         cout << endl;
        //     }

        //     for(auto &p : population.population){
        //         cout << p.min_max_cost << endl;
        //     }
        // }

        auto end_itertation = chrono::high_resolution_clock::now();
        float iteration_time = chrono::duration_cast<chrono::nanoseconds>(end_itertation - start_iteration).count();
        iteration_time *= 1e-9;

        cout << "Iteration " << iteration << ' ' << gbest.min_max_cost << ' ' << ibest.min_max_cost << ' ' << no_improve << ' ' << iteration_time << endl;

        float program_time = chrono::duration_cast<chrono::nanoseconds>(end_itertation - start_time).count();
        program_time *= 1e-9;

        if(cutoff_time != -1 && program_time > cutoff_time) break;

        for(int i = 0; i < n; i++){
            sort(graph.closest[i].begin(), graph.closest[i].end(), [&](const int &a, const int &b){
                if(gbest.assigned[a] != gbest.assigned[b])
                    return gbest.assigned[a] < gbest.assigned[b];
                return gbest.position[a] < gbest.position[b];
            });
        }
    }

    cout << "Result: " << gbest.min_max_cost << endl;
}
