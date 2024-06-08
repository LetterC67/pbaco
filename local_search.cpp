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

bool Ant::swap(_tour &a, _tour &b){
    bool swapped = false;

    for(int i = 1; i < a.size() - 1; i++){
        for(int j = 1; j < b.size() - 1; j++){
            float delta_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][b[j]] + (*distance)[b[j]][a[i + 1]];
            float delta_b = -(*distance)[b[j - 1]][b[j]] - (*distance)[b[j]][b[j + 1]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j + 1]]; 
            if(delta_a < 0 && delta_b < 0){
                swapped = true;
                std::swap(a[i], b[j]);
                a.cost += delta_a;
                b.cost += delta_b;
            }
        }
    }

    return swapped;
}

bool Ant::relocate(_tour &a, _tour &b){
    bool relocated = false;

    for(int i = 1; i < a.size() - 1; i++){
        for(int j = 1; j < b.size() - 1; j++){
            float delta_decrease_a = -(*distance)[a[i - 1]][a[i]] - (*distance)[a[i]][a[i + 1]] + (*distance)[a[i - 1]][a[i + 1]];
            float delta_increase_b = -(*distance)[b[j - 1]][b[j]] + (*distance)[b[j - 1]][a[i]] + (*distance)[a[i]][b[j]];

            if(max(a.cost + delta_decrease_a, b.cost + delta_increase_b) < max(a.cost, b.cost)){
                relocated = true;
                a.cost += delta_decrease_a;
                b.cost += delta_increase_b;
            
                b.tour.insert(b.begin() + j, a[i]);
                a.tour.erase(a.begin() + i);
                break;
            }
        }
    }

    return relocated;
}

bool Ant::swap_tail(_tour &a, _tour &b){
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

    for(int i = 1; i < a.size() - 1; i++){
        for(int j = 1; j < b.size() - 1; j++){
            float new_cost_a = pref_a[i - 1] + suf_b[j] + (*distance)[a[i - 1]][b[j]];
            float new_cost_b = pref_b[j - 1] + suf_a[i] + (*distance)[b[j - 1]][a[i]];
        
            if(max(new_cost_a, new_cost_b) < max(a.cost, b.cost)){
                a.cost = new_cost_a;
                b.cost = new_cost_b;

                vector<int> c = a.tour, d = b.tour;
                while(a.size() > i) a.tour.pop_back();
                while(b.size() > j) b.tour.pop_back();

                for(int k = i; k < c.size(); k++) b.push_back(c[k]);
                for(int k = j; k < d.size(); k++) a.push_back(d[k]);

                return true;
            }

            new_cost_a = pref_a[i - 1] + pref_b[j - 1] + (*distance)[a[i - 1]][b[j - 1]];
            new_cost_b = suf_a[i] + suf_b[j] + (*distance)[a[i]][b[j]];

            if(max(new_cost_a, new_cost_b) < max(a.cost, b.cost)){
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

                return true;
            }
        }
    }

    return false;
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

void Ant::or_opt(_tour &tour){
    for(int opt_size = 1; opt_size <= OPT_SIZE; opt_size++){
        for(int i = 1; i < (int)tour.size() - opt_size ; i++){
            for(int j = 0; j < (int)tour.size() - 2; j++){
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
    for(auto &tour : tours){
        two_opt(tour);
        or_opt(tour);
    }
}

void Ant::inter_tour_optimization_del(){
    vector<int> can(del.begin(),del.end());
    shuffle(can.begin(), can.end(), rng);

    bool loop = false;

    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                loop |= swap(tours[can[i]], tours[can[j]]);
        }
    }

    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                loop |= relocate(tours[can[i]], tours[can[j]]);
        }
    }
    
    
    for(int i = 0; i < can.size(); i++){
        for(int j = 0; j < can.size(); j++){
            if(i != j)
                loop |= swap_tail(tours[can[i]], tours[can[j]]);
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
            loop |= swap(tours[ord[i]], tours[ord[tour]]);   
    }

    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= relocate(tours[ord[i]], tours[ord[tour]]); 
    }

    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= swap_tail(tours[ord[i]], tours[ord[tour]]); 
    }

    for(int i = 0; i < tours.size(); i++){
        for(int tour = 0; tour < tours.size(); tour++)
        if(tour != i)
            loop |= swap_tail(tours[ord[i]], tours[ord[tour]]); 
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
}