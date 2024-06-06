#include <bits/stdc++.h>
#include "ant.h"
#include "parameters.h"

using namespace std;

struct state{
    array<double, 2> dp;
    state *trace;
    char update_type;
    state(){
        update_type = -1;
        dp = {1e9, 1e9};
    }
};


bool set_min_max(array<double,2> &a, array<double,2> b){
    if(max(a[0], a[1]) > max(b[0], b[1])){
        a = b;
        return true;
    }
    return false;
}

bool set_min_sum(array<double,2> &a, array<double,2> b){
    if(a[0] + a[1] > b[0] + b[1]){
        a = b;
        return true;
    }
    return false;
}

bool set_avg(array<double,2> &a, array<double,2> b){
    if(sqrt(a[0] * a[0] + a[1] * a[1]) > sqrt(b[0] * b[0] + b[1] * b[1])){
        a = b;
        return true;
    }
    return false;
}



bool (*objective_functions[3])(array<double,2> &, array<double,2> ) = {&set_min_max, &set_min_sum, &set_avg};

vector<vector<int>> trace(int i, int j, int k, int part, int type, vector<vector<vector<vector<vector   <state>>>>> &dp, vector<int> a[]){
    state* s = &dp[i][j][k][part][type];
    vector<vector<int>> res(2, vector<int>());

    array<int, 2> pos = {i,j};

    // for(int &d : a[0] ) cout << d << ' ';
    // cout << endl;
    // for(int &d : a[1] ) cout << d << ' ';
    // cout << endl;

    while(s -> update_type != -2){
        //  cout << i << ' ' << j << ' ' <<(int) s -> update_type << ' ' << s -> i << ' ' << s -> j << ' ' << s << ' ' << s -> dp[0] << ' ' << s -> dp[1] << ' ' << tour_length(res[0]) << ' ' << tour_length(res[1]) << endl;;
        if(i < -10) exit(0);
        if(s -> update_type == 0){
            res[k].push_back(a[0][i]);
            i--;
        }
        else if(s -> update_type == 1){
            res[!k].push_back(a[1][j]);
            j--;
        }
        else if(s -> update_type == 2){
            res[k].push_back(a[0][i]);
            res[!k].push_back(a[1][j]);
            k = !k;
            i--;
            j--;
        }
        else if(s -> update_type >= 100){
            int sz = s -> update_type - 100;
            res[!k].push_back(a[1][j]);
            res[k].push_back(a[0][i]);
            for(int p = i - 1; p >= i - sz ; p--){
                res[!k].push_back(a[0][p]);
            }
            i -= sz + 1;
            j--;
        }
        else if(s -> update_type >= 10){
            int sz = s -> update_type - 10;
            // cout << sz << endl;
            res[!k].push_back(a[1][j]);
            res[k].push_back(a[0][i]);
            for(int p = j - 1; p >= j - sz ; p--){
            //    cout << p << ' ' << a[1][p] << endl;
                res[k].push_back(a[1][p]);
            //    cout << "mim " << endl;
            }
            //   cout << "Hehe" << endl;
            i--;
            j -= sz + 1;
        }
        s = s -> trace;
    }


    res[0].push_back(DEPOT);
    res[1].push_back(DEPOT);

    return res;
}

void Ant::run_dp(_tour &x, _tour &y){
    double partition_size = ceil(max(x.cost, y.cost) * 1.1 / (double)DP_PARTITION_COUNT);
    vector<int> a[2] = {x.tour, y.tour};

    vector<vector<vector<vector<vector<state>>>>> dp(a[0].size() + 2, 
               vector<vector<vector<vector<state>>>>    (a[1].size() + 2,
                      vector<vector<vector<state>>>     (2,
                             vector<vector<state>>      (DP_PARTITION_COUNT,
                                    vector<state>       (3)))));

    dp[0][0][0][0][0].dp = {0,0};
    dp[0][0][0][0][0].update_type = -2;
    for(int i = 0; i < a[0].size(); i++){
        for(int j = 0; j < a[1].size(); j++){
            array<int, 2> pos = {i,j};
            for(int k = 0; k < 2; k++){
                for(int p = 0; p <  DP_PARTITION_COUNT; p++){
                    for(int type = 0; type < 3; type++){
                        auto &D = dp[i][j][k][p][type];
                        if(D.update_type == -1 && D.update_type != -2){
                            continue;  
                        }
                        if(i < a[0].size() - 1){    
                            for(int upd = 0; upd < 3; upd++){
                                auto new_cost = D.dp;
                                new_cost[k] += (*distance)[a[0][i]][a[0][i + 1]];
                                int part = new_cost[0] / partition_size;
                                if(part >= DP_PARTITION_COUNT) continue;
                                auto &d = dp[i + 1][j][k][part][type];
                                if((*(objective_functions[upd]))(d.dp, new_cost)){
                                    d.trace = &D;
                                    d.update_type=0;
                                }
                            }
                        }

                        if(j < a[1].size() - 1){
                            for(int upd = 0; upd < 3; upd++){
                                auto new_cost = D.dp;
                                new_cost[!k] += (*distance)[a[1][j]][a[1][j + 1]];
                                int part = new_cost[0] / partition_size;
                                if(part >= DP_PARTITION_COUNT) continue;
                                auto &d = dp[i][j + 1][k][part][type];
                                if((*(objective_functions[upd]))(d.dp, new_cost)){
                                    d.trace = &D;
                                    d.update_type=1;
                                }
                            }
                        }

                        if(i < a[0].size() - 1 && j < a[1].size() - 1){
                            for(int upd = 0; upd < 3; upd++){
                                auto new_cost = D.dp;
                                new_cost[k] += (*distance)[a[0][i]][a[1][j+ 1]];
                                new_cost[!k] += (*distance)[a[1][j]][a[0][i + 1]];
                                int part = new_cost[0] / partition_size;
                                if(part >= DP_PARTITION_COUNT) continue;
                                auto &d = dp[i + 1][j + 1][!k][part][type];
                                if((*(objective_functions[upd]))(d.dp, new_cost)){
                                    d.trace = &D;
                                    d.update_type=2;
                                }
                            }
                        }

                        if(i < a[0].size() - 1 ){
                            for(int upd = 0; upd < 3; upd++){
                                int last = a[0][i];
                                double cost = 0;
                                for(int l = j + 1; l < min(j + 5, (int)a[1].size() - 1); l++){
                                    cost += (*distance)[last][a[1][l]];
                                    last = a[1][l];
                                    auto new_cost = D.dp;
                                    new_cost[k] += cost + (*distance)[last][a[0][i + 1]];
                                    new_cost[!k] += (*distance)[a[1][j]][a[1][l + 1]];
                                    int part = new_cost[0] / partition_size;
                                    if(part >= DP_PARTITION_COUNT) continue;
                                    auto &d = dp[i + 1][l + 1][k][part][type];
                                    if((*(objective_functions[upd]))(d.dp, new_cost)){
                                        d.trace = &D;
                                        d.update_type=l-j + 10;
                                    }
                                }
                            }
                        }

                        if(j < a[1].size() - 1 ){
                            for(int upd = 0; upd < 3; upd++){
                                int last = a[1][j];
                                double cost = 0;
                                for(int l = i + 1; l < min(i + 5, (int)a[0].size() - 1); l++){
                                    cost += (*distance)[last][a[0][l]];
                                    last = a[0][l];
                                    auto new_cost = D.dp;
                                    new_cost[k] += (*distance)[a[0][i]][a[0][l + 1]]; 
                                    new_cost[!k] += cost + (*distance)[last][a[1][j + 1]];
                                    int part = new_cost[0] / partition_size;
                                    if(part >= DP_PARTITION_COUNT) continue;
                                    auto &d = dp[l + 1][j + 1][k][part][type];
                                    if((*(objective_functions[upd]))(d.dp, new_cost)){
                                        d.trace = &D;
                                        d.update_type=l-i + 100;
                                    }
                                }
                            }
                        }
                            
                    }
                }
            }
        }
    }

    array<double, 2> best = {1e9,1e9};
    int kk, pp, tt;
    for(int k = 0; k < 2; k++){
        for(int part = 0; part < DP_PARTITION_COUNT; part++){
            for(int i = 0; i < 3; i++){
                if(set_min_max(best, dp[a[0].size() - 1][a[1].size() - 1][k][part][i].dp)){
                    kk = k;
                    pp = part;
                    tt = i;
                }
            }
        }
    }

    cout << "DP: Old " << x.cost << ' ' << y.cost << " | New: " << best[0] << ' ' << best[1] << endl;
    if(max(best[0], best[1]) + 1e-6 > max(x.cost, y.cost)){
        return;
    }
    cout << "OK" << endl;

    auto res = trace(a[0].size() - 1, a[1].size() - 1, kk, pp, tt, dp, a);
    x.tour = res[0];
    y.tour = res[1];
    
    x.cost = tour_length(x);
    y.cost = tour_length(y);
}


void Ant::dp(){
    int salesmen = tours.size();

    vector<vector<int>>  dis(salesmen, vector<int>(salesmen));
    for(int i = 0; i < salesmen; i++)
        for(int j = i + 1; j < salesmen; j++)
            dis[i][j] = dis[j][i] = tour_distance(tours[i], tours[j]);

    for(int i = 0; i < salesmen; i++){
        vector<int> v;
        for(int j = 0; j < salesmen; j++){
            if(i != j)
                v.push_back(j);
        }
        sort(v.begin(), v.end(), [&](const int &a, const int &b){
            return (*distance)[i][a] < (*distance)[i][b];
        });

        for(int j = 0; j < (int)ceil(DP_PERCENTAGE * salesmen); j++){
            run_dp(tours[i], tours[v[j]]);
        }
    }

}