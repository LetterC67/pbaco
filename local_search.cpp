#include <bits/stdc++.h>
#include "ant.h"
#include "parameters.h"
#include "random.h"
using namespace std;


template<typename T>
void move_segment(vector<T>& vec, int l, int r, int i) {
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
    vec.insert(vec.begin() + i + 1, segment.begin(), segment.end());
}

float Ant::tour_length(_tour &tour){
    float cost = 0;
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
            float delta_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][b[j]] + (*distance)[b[j]][a[i + 1]];
            float delta_b = -(*distance)[b[j - 1]][b[j]] - (*distance)[b[j]][b[j + 1]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j + 1]]; 
            if(delta_a < 0 && delta_b < 0){
                // if(a.cost + b.cost < min_cost){
                //     min_cost = a.cost + b.cost;
                //     ii = i;
                //     jj = j;
                // }
                std::swap(assigned[a[i]], assigned[b[j]]);
                std::swap(position[a[i]], position[b[j]]);
                std::swap(a[i], b[j]);
                a.cost += delta_a;
                b.cost += delta_b;
                swapped = true;
            }
        }
    }

    // if(ii == -1) return false;

    // int i = ii, j = jj;
    // float delta_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][b[j]] + (*distance)[b[j]][a[i + 1]];
    // float delta_b = -(*distance)[b[j - 1]][b[j]] - (*distance)[b[j]][b[j + 1]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j + 1]]; 
    

    return swapped;
}

bool Ant::relocate(_tour &a, _tour &b, int idx_a, int idx_b){
    if(a.cost < b.cost ) return false;

    bool relocated = false;
    double min_cost = 1e9;
    int ii = -1, jj = -1;

    for(int i = 1; i < a.size() - 1; i++){
        for(int &u : (*graph).closest[a[i]]){
            if(assigned[u] != idx_b) continue;
            int j = position[u];
            float delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
            float delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

            if(delta_decrease_a + delta_increase_b < 0){
                if(min_cost > delta_decrease_a + delta_increase_b){
                    min_cost = delta_decrease_a + delta_increase_b;
                    ii = i;
                    jj = j;
                }
                
                break;
            }
        }
    }

    if(ii == -1) return false;

    int i = ii, j = jj;

    float delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
    float delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

    a.cost += delta_decrease_a;
    b.cost += delta_increase_b;

    b.tour.insert(b.begin() + j, a[i]);
    a.tour.erase(a.begin() + i);

    retag(idx_a);
    retag(idx_b);

    return true;
}

bool Ant::swap_tail(_tour &a, _tour &b, int idx_a, int idx_b){
    bool swapped = false;

    vector<float> pref_a(a.size()), suf_a(a.size()), pref_b(b.size()), suf_b(b.size());
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
            float new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
            float new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];
        
            float dak = (*distance)[a[i - 1]][b[j]] + (*distance)[b[j - 1]][a[i]];
            float mim = (*distance)[a[i - 1]][b[j - 1]]  + (*distance)[a[i]][b[j]];

            if(dak < mim && max(new_cost_a, new_cost_b) < max(a.cost, b.cost)){
                if(new_cost_a + new_cost_b < min_cost){
                    min_cost = new_cost_a + new_cost_b;
                    ii = i;
                    jj = j;
                    type = 0;
                }
            }

            new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
            new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

            if(mim < dak && max(new_cost_a, new_cost_b) < max(a.cost, b.cost)){
                if(new_cost_a + new_cost_b < min_cost){
                    min_cost = new_cost_a + new_cost_b;
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
        float new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
        float new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];

        a.cost = new_cost_a;
        b.cost = new_cost_b;

        vector<int> c = a.tour, d = b.tour;
        while(a.size() > i) a.tour.pop_back();
        while(b.size() > j) b.tour.pop_back();

        for(int k = i; k < c.size(); k++) b.push_back(c[k]);
        for(int k = j; k < d.size(); k++) a.push_back(d[k]);
    }else{
        float new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
        float new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];
        
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
    float x, y;
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

float get_delta(int a, int b, int c, int d, _tour &tour, vector<vector<float>> *distance){
    return (*distance)[tour[a]][tour[c]] - (*distance)[tour[a]][tour[b]] + (*distance)[tour[b]][tour[d]] - (*distance)[tour[c]][tour[d]];
    
}

struct sweepline_event{
    float x;
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


    for(int i = 0; i < segment.size(); i++){
        event.push_back({(*graph).x[segment[i].u], i, 1});
        event.push_back({(*graph).x[segment[i].v], i, 0});
    }

    sort(event.begin(), event.end(), [](const sweepline_event &a, const sweepline_event &b){
        if(a.x != b.x) return a.x < b.x;
        return a.type < b.type;
    });

    bool used = false;

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
                
                float delta = get_delta(aa, bb, c, d, tour, distance);
                if(delta < 0){
                    tour.cost += delta;
                    reverse(tour.begin() + bb, tour.begin() + c + 1);
                    retag(idx);
                    used = true;
                    break;
                }
            }
            current.insert(e.index);
        }else{
            current.erase(e.index);
        }
    }
    
    return used;
}

void Ant::two_opt(_tour &tour){
    for(int l = 1; l < tour.size() - 1; l++)
        for(int r = l + 1; r < tour.size() - 1; r++){
            if((*distance)[tour[l - 1]][tour[r]] - (*distance)[tour[l - 1]][tour[l]] + (*distance)[tour[l]][tour[r + 1]] - (*distance)[tour[r]][tour[r + 1]] < 0){
                tour.cost += (*distance)[tour[l - 1]][tour[r]] - (*distance)[tour[l - 1]][tour[l]] + (*distance)[tour[l]][tour[r + 1]] - (*distance)[tour[r]][tour[r + 1]] ;
                reverse(tour.begin() + l, tour.begin() + r + 1);
            }
        }
}

void Ant::or_opt(_tour &tour, int idx){
    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            for(auto &jj : (*graph).closest[tour[i + opt_size - 1]]){
                if(assigned[jj] != idx) continue;
                int j = position[jj] - 1;
                if(i - 1 <= j && j <= i + opt_size - 1) continue;
                float delta = (*distance)[tour[j]][tour[i]] + (*distance)[tour[i + opt_size - 1]][tour[j + 1]] + (*distance)[tour[i - 1]][tour[i + opt_size]]
                    - (*distance)[tour[j]][tour[j + 1]] - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]];

                if(delta < 0){
                    move_segment(tour.tour, i, i + opt_size - 1, j);
                    tour.cost += delta;
                }
            }
        }
    }
}

void Ant::or_opt(_tour &tour){
    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            for(int j = 1; j < tour.size() - 1; j++){
                if(i - 1 <= j && j <= i + opt_size - 1) continue;
                float delta = (*distance)[tour[j]][tour[i]] + (*distance)[tour[i + opt_size - 1]][tour[j + 1]] + (*distance)[tour[i - 1]][tour[i + opt_size]]
                    - (*distance)[tour[j]][tour[j + 1]] - (*distance)[tour[i - 1]][tour[i]] - (*distance)[tour[i + opt_size - 1]][tour[i + opt_size]];

                if(delta < 0){
                    move_segment(tour.tour, i, i + opt_size - 1, j);
                    tour.cost += delta;
                }
            }
        }
    }
}

int Ant::longest_tour_index(){
    float cost = 0;
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
    float cost = 1e18;
    int index = 0;
    for(int i = 0; i < tours.size(); i++){
        if(tours[i].cost < cost){
            cost = tours[i].cost;
            index = i;
        }
    }
    return index;
}


void Ant::intra_tour_optimization(){
    int idx = 0;
    for(auto &tour : tours){
        // two_opt_sweepline(tour, idx);
        // two_opt_sweepline(tour, idx);
        // two_opt_sweepline(tour, idx);
        if(tour.size() > 500)two_opt_sweepline(tour, idx);
        else two_opt(tour);
        if(tour.size() > 500)
        or_opt(tour, idx);
        else or_opt(tour);
        idx++;
    }
    for(int i = 0; i < tours.size(); i++)
        retag(i);
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
                loop |= relocate(tours[can[i]], tours[can[j]], can[i], can[j]);
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
            loop |= swap(tours[ord[i]], tours[ord[tour]], ord[tour]);   
    }

    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            while(relocate(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour])){}
    }
    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= swap_tail(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour]); 
    }
    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= swap_tail(tours[ord[i]], tours[ord[tour]], ord[i], ord[tour]); 
    }
}


void Ant::local_search(){
    inter_tour_optimization_del();
    intra_tour_optimization();
    for(int i = 0; i < LOCAL_SEARCH_ITERATIONS; i++){
        for(int j = 0; j < INTER_TOUR_INTERATIONS; j++)
            inter_tour_optimization();

        intra_tour_optimization();
    }

    for(auto &tour : tours){
        tour.cost = tour_length(tour);
        // if(abs(tour_length(tour) - tour.cost) > 1e-3){
        //     cout << tour_length(tour) - tour.cost << endl;
        // }
        // assert(abs(tour_length(tour) - tour.cost) <= 1e-3);
    }    
     
}