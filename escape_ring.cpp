#include <bits/stdc++.h>
#include "parameters.h"
#include "ant.h"
#include "random.h"

using namespace std;

bool Ant::is_inside(int p, _tour &a){
    bool inside = false;

    for(int i = 0; i < a.size() - 1; i++){
        int p1 = a[i], p2 = a[i + 1];
        auto &distance = graph -> distance;
        auto &x = graph -> x;
        auto &y = graph -> y;
        if(y[p] > min(y[p1], y[p2]) && y[p] <= max(y[p1], y[p2])){
            if(x[p] <= max(x[p1], x[p2])){
                if(y[p1] != y[p2]){
                    double xinters = (y[p] - y[p1]) * (x[p2] - x[p1]) / (y[p2] - y[p1]) + x[p1];
                    if(x[p1] == x[p2] || x[p] <= xinters){
                        inside = !inside;
                    }
                }
            }
        }
    }

    return inside;
}

bool Ant::is_inside(_tour &a, _tour &b){
    int inside_count = 0;

    for(int i = 0; i < 10; i++)
        inside_count += is_inside(a.tour[rand_range(1, a.tour.size() - 2)], b);

    return inside_count == 10;
}

void Ant::remove_ring(_tour &a, _tour &b, int idx_a, int idx_b){
    double max_cost = 1e9;
    
    vector<double> pref_a(a.size()), suf_a(a.size()), pref_b(b.size()), suf_b(b.size());

    for(int i = 1; i < a.size(); i++){
        pref_a[i] = pref_a[i - 1] + (*distance)[a[i - 1]][a[i]];
    }
    for(int i = 1; i < b.size(); i++){
        pref_b[i] = pref_b[i - 1] + (*distance)[b[i - 1]][b[i]];
    }
    for(int i = a.size() - 2; i >= 0; i--){
        suf_a[i] = suf_a[i + 1] + (*distance)[a[i + 1]][a[i]];   
    }
    for(int i = b.size() - 2; i >= 0; i--){
        suf_b[i] = suf_b[i + 1] + (*distance)[b[i + 1]][b[i]];   
    }

    int ii = -1, jj = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double t1 = (*distance)[a[i - 1]][b[j]] + (*distance)[b[j - 1]][a[i]];
            double t2 = (*distance)[a[i - 1]][b[j - 1]] + (*distance)[a[i]][b[j]];
            
            if(t1 < t2){
                double new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
                double new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

                if(max(new_cost_a, new_cost_b) < max_cost){
                    ii = i;
                    jj = j;
                    type = 0;
                    max_cost = max(new_cost_a, new_cost_b);
                }
            }else{
                double new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
                double new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

                if(max(new_cost_a, new_cost_b) < max_cost){
                    ii = i;
                    jj = j;
                    type = 1;
                    max_cost = max(new_cost_a, new_cost_b);
                }
            }
        }
    }

    if(type == -1) return;

    int i = ii, j = jj;

    if(type == 0){
        double new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
        double new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

      //  cout << a.cost << ' ' << b.cost << ' ' << new_cost_a << ' ' << new_cost_b << endl;

        a.cost = new_cost_a;
        b.cost = new_cost_b;

        vector<int> c = a.tour, d = b.tour;
        while(a.size() > i) a.tour.pop_back();
        while(b.size() > j) b.tour.pop_back();

        for(int k = i; k < c.size(); k++) b.push_back(c[k]);
        for(int k = j; k < d.size(); k++) a.push_back(d[k]);

        retag(idx_a);
        retag(idx_b);
    }else{
        double new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
        double new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

        a.cost = new_cost_a;
        b.cost = new_cost_b;

        vector<int> c = vector<int>(a.tour.begin() + i, a.tour.end()),
                    d = vector<int>(b.tour.begin() + j, b.tour.end());
        a.tour.erase(a.tour.begin() + i, a.tour.end());
        b.tour.erase(b.tour.begin() + j, b.tour.end());

        reverse(b.begin(), b.end());;
        a.tour.insert(a.end(), b.begin(), b.end());
        
        reverse(c.begin(), c.end());
        b.tour = c;
        b.tour.insert(b.end(), d.begin(), d.end());

        retag(idx_a);
        retag(idx_b);
    }
}

void Ant::escape_ring(){
    int salesmen = tours.size();

    intra_tour_optimization();


    for(int i = 0; i < salesmen; i++){
        for(int j = i; j < salesmen; j++){
            if(i != j ){
            //     ofstream f("temp", ofstream::out);
            // // auto fp = freopen("temp", "w", stdout);
            //     for(auto &tour : tours){
            //         for(int &d : tour){
            //             f << d << ' ';
            //         }
            //         f << endl;
            //     }
       //         cout << "Before " << tours[i].cost << ' ' << tours[j].cost << ' ';
                remove_ring(tours[i], tours[j], i, j);
         //       cout << " After " <<  tours[i].cost << ' ' << tours[j].cost << endl;
                // f << endl;
                // for(auto &tour : tours){
                //     for(int &d : tour){
                //         f << d << ' ';
                //     }
                //     f << endl;
                // }
            }
        }
    }

    

    for(int i = 0; i < LOCAL_SEARCH_ITERATIONS; i++){
        inter_tour_optimization(false);
        intra_tour_optimization();
    }

    calculate_result();
    //fclose(fp);
}