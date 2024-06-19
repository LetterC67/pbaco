#include <bits/stdc++.h>
#include <omp.h>    
#include <stdlib.h>
#include <getopt.h>
#include "solver.h"
#include "graph.h"
#include "parameters.h"
#include "stat.h"
#include "ga_tsp/run_tsp.h"

using namespace std;

static struct option long_options[] = {
    {"help",    no_argument,       0, 'h'},
    {"dataset", required_argument, 0, 'd'},
    {"salesmen", required_argument, 0, 's'},
    {"time",  optional_argument, 0, 'c'},
    {"iteration", optional_argument, 0, 'i'},
    {"thread", optional_argument, 0, 't'},
    {"variation", optional_argument, 0, 'v'},
    {"run", optional_argument, 0, 'r'},
    {0,         0,                 0,  0 }
};

vector<int> min_pop = {10, 15, 20};
vector<int> max_pop = {100};
vector<double> pop_arg = {1.15, 1.2, 1.25};


int main(int argc, char **args){
    ios_base::sync_with_stdio(0); cin.tie(0);

    int option_index = 0;
    int c;

    string dataset;
    string variation;
    int salesmen = -1;
    int cutoff_interation = 1e9;
    int cutoff_time = -1;
    int threads = 1;
    int run = 30;

    while (1) {
        c = getopt_long(argc, args, "hd:s:c:i:t:v:r:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 'h':
                printf("Usage: %s [options]\n", args[0]);
                printf("  -h, --help       Show this help message\n");
                printf("  -d, --dateset    Path to the dataset\n");
                printf("  -s, --salesmen   The number of salesmen\n");
                printf("  -t, --time       Cutoff time in second(s)\n");
                printf("  -i, --iteration  Cutoff iteration\n");
                printf("  -t, --thread     The number of threads\n");
                printf("  -r, --run        The number of run\n"),
                printf("  -v, --variation  Variation\n");
                printf("\n");
                printf("Dataset and salesmen are required.\n");
                printf("If neither the cutoff time nor the iteration is specified, the program will default to using a cutoff of 1000 iterations. If both are specified, then both will be used as stopping condition.\n");
                printf("If the number of run is not specified, the program will default to running 30 times.\n");
                exit(0);

            case 'd':
                dataset = string(optarg);
                break;

            case 's':
                salesmen = stoi(optarg);
                break;

            case 'c':
                cutoff_time = stoi(optarg);
                break;
            
            case 'i':
                cutoff_interation = stoi(optarg);
                break;

            case 't':
                threads = stoi(optarg);
                break;

            case 'v':
                variation = string(optarg);
                break;

            case 'r':
                run = stoi(optarg);
                break;

            case '?':
                fprintf(stderr, "Unknown option: -%c\n", optopt);
                break;

            case ':':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);

        }
    }

    if(cutoff_interation == 1e9 && cutoff_time == -1){
        cutoff_interation = 1000;
    }

    if(salesmen == -1 || dataset.empty()){
        cout << "Please specify the dataset and the number of salesmen." << endl;;
        return 0;
    }

    cout << dataset << ' ' << salesmen << endl;

    omp_set_num_threads(threads);


    system("mkdir convergence");
    system("mkdir result");

    Graph graph;
    graph.load_data(dataset);


    for(auto &mn : min_pop){
        for(auto &mx : max_pop){
            for(auto &pop : pop_arg){
                MIN_POPULATION_SIZE = mn;
                MAX_POPULATION_SIZE = mx;
                DIFFERENCE_COEFFICIENT = pop;
                variation = "tuning_" + to_string(mn) + "_" + to_string(mx) + "_" + to_string(pop);
                Stat stat(variation, graph, salesmen);
                for(int i = 0; i < run; i++){
                    cout << "Run " << i + 1 << endl;
                    mTSPSolver solver(graph, salesmen, cutoff_time, cutoff_interation);
                    solver.solve(stat);
                    stat.write_result(solver.gbest, variation, graph, salesmen, i + 1);
                }
                stat.write_convergence(variation, graph, salesmen);
                stat.write_result_complete(variation, graph, salesmen);
            }
        }
    }

    

    

    return 0;
}