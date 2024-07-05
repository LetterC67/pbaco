#include <bits/stdc++.h>
#include "ant.h"
#include "parameters.h"
#include "random.h"
using namespace std;


template<typename T>
void move_segment(vector<T>& vec, int l, int r, int i, bool type = false) {
    if (l < 0 || r >= vec.size() || i < 0 || i >= vec.size()) {
        cout << "Invalid indices\n";
        return;
    }

    if (l > r || i >= l && i <= r) {
        cout << "Invalid segment or destination position " << l << ' ' << r << ' ' << i << endl;;
        return;
    }

    vector<T> segment(vec.begin() + l, vec.begin() + r + 1);
    vec.erase(vec.begin() + l, vec.begin() + r + 1);

    if (i > r) {
        i -= (r - l + 1);
    }

    assert(i < vec.size());
    if(type)
    reverse(segment.begin(), segment.end());
    vec.insert(vec.begin() + i + 1, segment.begin(), segment.end());
}
double Ant::tour_length(_tour &tour){
    double cost = 0;
    for(int i = 0; i < tour.size() - 1; i++){
        cost += (*distance)[tour[i]][tour[i + 1]];
    }
    return cost;
}

bool Ant::swap(_tour &a, _tour &b, int idx_b){
    bool swapped = false;

    int ii = -1, jj = -1;
    double min_cost = 1e9;

    for(int i = 1; i < a.size() - 1; i++){
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double delta_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][b[j]] + (*distance)[b[j]][a[i + 1]];
            double delta_b = -(*distance)[b[j - 1]][b[j]] - (*distance)[b[j]][b[j + 1]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j + 1]]; 
            if(delta_a < -1e-5 && delta_b < -1e-5){
                if(delta_a + delta_b < min_cost){
                    min_cost = delta_a + delta_b;
                    ii = i;
                    jj = j;
                }
               
            }
        }
    }

    if(ii == -1) return false;

    int i = ii, j = jj;
    double delta_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][b[j]] + (*distance)[b[j]][a[i + 1]];
    double delta_b = -(*distance)[b[j - 1]][b[j]] - (*distance)[b[j]][b[j + 1]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j + 1]]; 
    

    std::swap(assigned[a[i]], assigned[b[j]]);
    std::swap(position[a[i]], position[b[j]]);
    std::swap(a[i], b[j]);
    a.cost += delta_a;
    b.cost += delta_b;

    return true;
}

bool Ant::relocate(_tour &a, _tour &b, int idx_a, int idx_b){
    if(a.cost < b.cost ) return false;

    bool relocated = false;
    double min_cost = 1e9;
    int ii = -1, jj = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
        double delta_decrease_a_2 = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] - (*distance)[a[i + 1]][a[i + 2]]  + (*distance)[a[i - 1]][a[i + 2]];
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

            if(max(a.cost + delta_decrease_a, b.cost + delta_increase_b) + 1e-4 < max(a.cost, b.cost)){
                if(min_cost > delta_increase_b + delta_decrease_a){
                    min_cost = delta_increase_b + delta_decrease_a;
                    ii = i;
                    type = 0;
                    jj = j;
                }
            }

            if(i < a.size() - 2){
                
                double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i + 1]][b[j]];

                double _delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
                int t = 1;

                if(delta_increase_b > _delta_increase_b){
                    t = 2;
                    delta_increase_b = _delta_increase_b; 
                }


                if(max(a.cost + delta_decrease_a_2, b.cost + delta_increase_b) + 1e-4 < max(a.cost, b.cost)){
                    if(min_cost > delta_increase_b + delta_decrease_a_2){
                        min_cost = delta_increase_b + delta_decrease_a_2;
                        ii = i;
                        type = t;
                        jj = j;
                    }
                }
            }
        }
    }

    if(ii == -1) return false;

    int i = ii, j = jj;

    if(type == 0){
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
        double delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

        a.cost += delta_decrease_a;
        b.cost += delta_increase_b;

        b.tour.insert(b.begin() + j, a[i]);
        a.tour.erase(a.begin() + i);
    }else{
        double delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] - (*distance)[a[i + 1]][a[i + 2]]  + (*distance)[a[i - 1]][a[i + 2]];
        double delta_increase_b;

        if(type == 1){
            delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i + 1]][b[j]];
        }else{
            delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i + 1]] + (*distance)[a[i]][a[i + 1]] + (*distance)[a[i]][b[j]];
        }

        a.cost += delta_decrease_a;
        b.cost += delta_increase_b;

        if(type == 1){
            b.tour.insert(b.begin() + j, a[i + 1]);
            b.tour.insert(b.begin() + j, a[i]);
        }else{
            b.tour.insert(b.begin() + j, a[i]);
            b.tour.insert(b.begin() + j, a[i + 1]);
        }
        a.tour.erase(a.begin() + i);
        a.tour.erase(a.begin() + i);
    }

    // assert(abs(a.cost - tour_length(a)) < 1e-3);
    // assert(abs(b.cost - tour_length(b)) < 1e-3);

    retag(idx_a);
    retag(idx_b);

    return true;
}

bool Ant::swap_tail(_tour &a, _tour &b, int idx_a, int idx_b){
    bool swapped = false;

    vector<double> pref_a(a.size()), suf_a(a.size()), pref_b(b.size()), suf_b(b.size());
    bool ok = false;
    //cout << "here" << endl;

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

    double min_cost = 1e9;
    int ii = -1, jj = -1, type = -1;

    for(int i = 1; i < a.size() - 1; i++){
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            double new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
            double new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

            if(max(new_cost_a, new_cost_b) + 1e-4 < max(a.cost, b.cost)){
                if(max(new_cost_a, new_cost_b) < min_cost){
                    min_cost = max(new_cost_a, new_cost_b);
                    ii = i;
                    jj = j;
                    type = 0;
                }
            }

            new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
            new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

            if(max(new_cost_a, new_cost_b) + 1e-4 < max(a.cost, b.cost)){
                if(max(new_cost_a, new_cost_b) < min_cost){
                    min_cost = max(new_cost_a, new_cost_b);
                    ii = i;
                    jj = j;
                    type = 1;
                }
            }
        }
    }

    if(type == -1) return false;

    int i = ii, j = jj;

    if(type == 0){
        double new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
        double new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

        a.cost = new_cost_a;
        b.cost = new_cost_b;

        vector<int> c = a.tour, d = b.tour;
        while(a.size() > i) a.tour.pop_back();
        while(b.size() > j) b.tour.pop_back();

        for(int k = i; k < c.size(); k++) b.push_back(c[k]);
        for(int k = j; k < d.size(); k++) a.push_back(d[k]);
    }else{
        double new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
        double new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];
        
        a.cost = new_cost_a;
        b.cost = new_cost_b;

        vector<int> c = vector<int>(a.tour.begin() + i, a.tour.end()),
                    d = vector<int>(b.tour.begin() + j, b.tour.end());
        a.tour.erase(a.tour.begin() + i, a.tour.end());
        b.tour.erase(b.tour.begin() + j, b.tour.end());

        reverse(b.begin(), b.end());
        a.tour.insert(a.end(), b.begin(), b.end());
        
        reverse(c.begin(), c.end());
        b.tour = c;
        b.tour.insert(b.end(), d.begin(), d.end());
    }

    retag(idx_a);
    retag(idx_b);
    return true;
}

struct Point{
    double x, y;
};

struct Segment{
    int u, v;

    Segment(int u, int v, Graph *graph): u(u), v(v){
        if((*graph).x[this -> u] > (*graph).x[this -> v]){
            swap(this -> u, this -> v);
        }
        //assert((*graph).x[this -> u] <= (*graph).x[this -> v]);
    }
};

double get_delta(int a, int b, int c, int d, _tour &tour, vector<vector<double>> *distance){
    return (*distance)[tour[a]][tour[c]] - (*distance)[tour[a]][tour[b]] + (*distance)[tour[b]][tour[d]] - (*distance)[tour[c]][tour[d]];
    
}

struct sweepline_event{
    double x;
    int index;
    bool type;
};

bool Ant::two_opt_sweepline(_tour &tour, int idx){
    vector<Segment> segment;

    for(int i = 0; i < tour.size() - 1; i++){
        segment.push_back(Segment(tour[i], tour[i + 1], graph));
    }

    unordered_set<int> current;

    vector<sweepline_event> event;
    event.reserve(segment.size() * 2);


    for(int i = 0; i < segment.size(); i++){
        event.push_back({(*graph).x[segment[i].u], i, 1});
        event.push_back({(*graph).x[segment[i].v], i, 0});
    }

    sort(event.begin(), event.end(), [](const sweepline_event &a, const sweepline_event &b){
        if(a.x != b.x) return a.x < b.x;
        return a.type < b.type;
    });

    double min_cost = 1e9;
    int l = -1, r = -1;

    for(auto &e : event){
        if(e.type){
            int a = position[segment[e.index].u], b = position[segment[e.index].v];
            if(a > b) std::swap(a, b);
            if(a + 1 != b) continue;
            /// cout << current.size() << endl;
            for(auto &i : current){
                int c = position[segment[i].u], d = position[segment[i].v];
                if(c > d) std::swap(c, d);
                if(c + 1 != d) continue;
                int aa = a, bb = b;
                if(aa > c) std::swap(aa, c), std::swap(bb, d);
                
                double delta = get_delta(aa, bb, c, d, tour, distance);
                if(delta < -1e-4){
                    if(delta < min_cost){
                        min_cost = delta;
                        l = bb, r = c;
                    }
                }
            }
            current.insert(e.index);
        }else{
            current.erase(e.index);
        }
    }

    if(l == -1) return false;
    double delta = get_delta(l - 1, l, r, r + 1, tour, distance);
    reverse(tour.begin() + l, tour.begin() + r + 1);
    tour.cost += delta;
    retag(idx);
    
    return true;
}

bool Ant::two_opt(_tour &tour){
    bool swapped = false;

    for(int l = 1; l < tour.size() - 1; l++)
        for(int r = l + 1; r < tour.size() - 1; r++){
            double t = (*distance)[tour[l - 1]][tour[r]] - (*distance)[tour[l - 1]][tour[l]] + (*distance)[tour[l]][tour[r + 1]] - (*distance)[tour[r]][tour[r + 1]];
            if(t < -1e-4){
                swapped = true;
                tour.cost += t;
                reverse(tour.begin() + l, tour.begin() + r + 1);
            }
        }

    return swapped;
}

bool Ant::or_opt(_tour &tour, int idx){
    bool improved = false;
    int _i = -1, _j = -1, _opt_size = -1, type = -1;
    double best = 1e9;

    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            for(auto &jj : (*graph).closest[tour[i]]){
                if(assigned[jj] != idx) continue;
                int j = position[jj];
                if(i - 1 <= j && j <= i + opt_size - 1) continue;
                double delta = (*distance)[tour[j]][tour[i]] + (*distance)[tour[i + opt_size - 1]][tour[j + 1]] + (*distance)[tour[i - 1]][tour[i + opt_size]]
                    - (*distance)[tour[j]][tour[j + 1]] - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]];
                
                double _delta = (*distance)[tour[j]][tour[i + opt_size - 1]] + (*distance)[tour[i]][tour[j + 1]] + (*distance)[tour[i - 1]][tour[i + opt_size]]
                    - (*distance)[tour[j]][tour[j + 1]] - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]];

                int t = 0;

                if(_delta < delta){
                    t = 1;
                    delta = _delta;
                }

                if(delta < -1e-4){
                    if(delta < best){
                        best = delta;
                        _i = i;
                        _j = j;
                        _opt_size = opt_size;
                        type = t;
                    }
                    
                }
            }
        }
    }

    if(_i == -1) return false;
    int i = _i, j = _j, opt_size = _opt_size;
//    cerr << i << ' ' << j << ' ' << type << ' ' << best <<endl;

    move_segment(tour.tour, i, i + opt_size - 1, j, type);
    tour.cost += best;
   // assert(abs(tour.cost - tour_length(tour)) < 1e-3);

    return true;
}

bool Ant::or_opt(_tour &tour){
    bool improved = false;

    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            for(int j = 1; j < tour.size() - 1; j++){
                if(i - 1 <= j && j <= i + opt_size - 1) continue;
                double delta = (*distance)[tour[j]][tour[i]] + (*distance)[tour[i + opt_size - 1]][tour[j + 1]] + (*distance)[tour[i - 1]][tour[i + opt_size]]
                    - (*distance)[tour[j]][tour[j + 1]] - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]];

                if(delta < -1e-4){
                    move_segment(tour.tour, i, i + opt_size - 1, j);
                    tour.cost += delta;
                    improved = true;
                }
            }
        }
    }

    return improved;
}

int Ant::longest_tour_index(){
    double cost = 0;
    int index = 0;
    for(int i = 0; i < tours.size(); i++){
        if(tours[i].cost > cost){
            cost = tours[i].cost;
            index = i;
        }
    }
    return index;
}

int Ant::shortest_tour_index(){
    double cost = 1e18;
    int index = 0;
    for(int i = 0; i < tours.size(); i++){
        if(tours[i].cost < cost){
            cost = tours[i].cost;
            index = i;
        }
    }
    return index;
}



bool Ant::intra_tour_optimization(){
    int idx = 0;
    bool improved = false;
    for(auto &tour : tours){
        // two_opt_sweepline(tour, idx);
        // two_opt_sweepline(tour, idx);
        // two_opt_sweepline(tour, idx);
        if(tour.size() > 300){
            if(two_opt_sweepline(tour, idx)) improved = true;
        }
        else if(two_opt(tour)) improved = true;
        if(tour.size() > 300)
        improved |= or_opt(tour, idx);
        else improved |=  or_opt(tour);
        idx++;
    }
    for(int i = 0; i < tours.size(); i++)
        retag(i);
    return improved;
}


bool Ant::intra_tour_optimization_del(){
    bool improved = false;

    for(auto &idx : del){
        auto &tour = tours[idx];
        if(tour.size() > 300){
            if(two_opt_sweepline(tour, idx)) improved = true;
        }
        else if(two_opt(tour)) improved = true;
        if(tour.size() > 300)
        improved |= or_opt(tour, idx);
        else improved |= or_opt(tour);
    }
    for(int i = 0; i < tours.size(); i++)
        retag(i);
    return improved;
}


void Ant::inter_tour_optimization_del(){
    vector<int> can(del.begin(),del.end());
    shuffle(can.begin(), can.end(), rng);

    bool loop = false;

    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                loop |= swap(tours[can[i]], tours[can[j]], can[j]);
        }
    }

    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                while(relocate(tours[can[i]], tours[can[j]], can[i], can[j])){}
        }
    }
    
    
    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                loop |= swap_tail(tours[can[i]], tours[can[j]], can[i], can[j]);
        }
    }
}

void Ant::inter_tour_optimization(){
    vector<int> ord;
    
    for(int i = 0; i < tours.size(); i++)
        ord.push_back(i);

    shuffle(ord.begin(), ord.end(), rng);
    
    bool loop = false;
    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            while(relocate(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour])){}
    }
    // for(int i = 0; i < tours.size(); i++){
    //     for(int tour = 0; tour < tours.size(); tour++)
    //     if(tour != i)
    //         while(swap(tours[ord[i]], tours[ord[tour]], ord[tour])){}
    // }

    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= swap_tail(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour]); 
    }
}


void Ant::local_search(){
    // intra_tour_optimization_del();    
    // vector<int> can(del.begin(),del.end());

    // for(int i = 0; i < 3; i++){
    //     bool improved = false;
    //     shuffle(can.begin(), can.end(), rng);
        
    //     if(i == 0){
    //         for(int i = 0; i < can.size(); i++){
    //             for(int j = 0; j < can.size(); j++){
    //                 if(i != j)
    //                     if(relocate(tours[can[i]], tours[can[j]], can[i], can[j])){
    //                         improved = true;
    //                     }
    //             }
    //         }
    //     }else if(i == 1){
    //         for(int i = 0; i < can.size(); i++){
    //             for(int j = 0; j < can.size(); j++){
    //                 if(i != j)
    //                     improved |= swap_tail(tours[can[i]], tours[can[j]], can[i], can[j]);
    //             }
    //         }
    //     }else{
    //         improved |= intra_tour_optimization_del();
    //     }

    //     if(improved) i = -1;
    // }

    //     vector<int> ord;
    
    // for(int i = 0; i < tours.size(); i++)
    //     ord.push_back(i);

    // while(1){
    //     bool improved = false;
    //     shuffle(ord.begin(), ord.end(), rng);

    //     for(int i = 0; i < tours.size(); i++){
    //         for(int tour = 0; tour < tours.size(); tour++)
    //             if(tour != i)
    //                 while(relocate(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour])){
    //                     improved = true;
    //                 }
    //     }

    //     for(int i = 0; i < tours.size(); i++){
    //         for(int tour = 0; tour < tours.size(); tour++)
    //         if(tour != i)
    //             improved |= swap_tail(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour]); 
    //     }

    //     improved |= intra_tour_optimization();

    //     if(!improved) break;
    // }
    vector<int> ord;
    
    for(int i = 0; i < tours.size(); i++)
        ord.push_back(i);

    
    for(int i = 0; i < 3; i++){
        bool improved = false;
        shuffle(ord.begin(), ord.end(), rng);
        
        if(i == 0){
            for(int i = 0; i < tours.size(); i++){
                for(int tour = 0; tour < tours.size(); tour++)
                    if(tour != i)
                        if(relocate(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour])){
                            improved = true;
                        }
            }
        }else if(i == 1){
            for(int i = 0; i < tours.size(); i++){
                for(int tour = 0; tour < tours.size(); tour++)
                if(tour != i)
                    improved |= swap_tail(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour]); 
            }
        }else{
            improved |= intra_tour_optimization();
        }

        if(improved) i = -1;

        for(auto &tour : tours){
            tour.cost = tour_length(tour);
        }
    }

        
     
}