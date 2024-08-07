#include <bits/stdc++.h>
#include "ant.h"
#include "random.h"
#include "ga_tsp/run_tsp.h"

using namespace std;

void Ant::retag(int index){
    for(int i = 1; i < tours[index].size() - 1; i++){
        assigned[tours[index][i]] = index;
        position[tours[index][i]] = i;
    }
}

void Ant::add(int salesman, int vertex){
    tours[salesman].cost += (*distance)[tours[salesman].back()][vertex];
    assigned[vertex] = salesman;
    position[vertex] = tours[salesman].size();
    tours[salesman].push_back(vertex);
}

void Ant::add(_tour &tour, int vertex){
    tour.cost += (*distance)[tour.back()][vertex];
    tour.push_back(vertex);
}

void Ant::end_tour(){
    min_max_cost = min_sum_cost = 0;

    for(auto &tour: tours){
        add(tour, 0);
        min_max_cost = max(min_max_cost, tour.cost);
        min_sum_cost += tour.cost;
    }
}

void Ant::calculate_result(){
    min_max_cost = min_sum_cost = 0;
    for(auto &tour : tours){
        min_max_cost = max(min_max_cost, tour.cost);
        min_sum_cost += tour.cost;
        sqrt_cost += tour.cost * tour.cost;
    }
    sqrt_cost = sqrt(sqrt_cost);
}

int count_similar(unordered_set<int> edges, vector<int> &tour, int n){
    int ans = 0;

    for(int i = 0; i < tour.size() - 1; i++){
        ans += edges.count(tour[i] * n + tour[i + 1]);
    }

    return ans;
}

void Ant::run_tsp(){
    for(auto &tour : tours){
        if(tour.size() <= 80) continue;

        vector<vector<double>> _distance(tour.size() - 1, vector<double>(tour.size() - 1));

        for(int i = 0; i < tour.size() - 1; i++){
            for(int j = 0; j < tour.size() - 1; j++){
                _distance[i][j] = _distance[j][i] = (*distance)[tour[i]][tour[j]];
            }
        }
        
        auto result = run(tour.size() - 1, _distance);

        //cout << "Old vs. new: " << tour.cost << ' ' << result.first << endl;
        if(result.first < tour.cost){
            tour.cost = result.first;
            auto _tour = result.second;
            while(_tour.front()){
                _tour.push_back(_tour.front());
                _tour.erase(_tour.begin());
            }

            vector<int> temp;
            for(int i = 0; i < tour.size() - 1; i++){
                temp.push_back(tour[_tour[i]]);
            }   
        
            temp.push_back(0);

            unordered_set<int> edges;

            for(int i = 0; i < tour.size(); i++){
                edges.insert(tour[i] * (*distance).size() + tour[i + 1]);
            }

            int no_rev = count_similar(edges, temp, (*distance).size());
            reverse(temp.begin(), temp.end());
            int rev = count_similar(edges, temp, (*distance).size());

            if(no_rev > rev)
                reverse(temp.begin(), temp.end());

            tour.tour = temp;
        }
    }

    for(int i = 0; i < tours.size(); i++){
        retag(i);
        //cout << "New cost " << i << ' ' << tours[i].cost << endl;
    }

    calculate_result();
}

Ant trim(Ant ant){
    int salesmen = ant.tours.size();
            
    int del_count = rng() % ((salesmen + 1) / 2) + 1;
    unordered_set<int> del_list;

    vector<int> v;
    for(int i = 0; i < salesmen; i++)
        if(!del_list.count(i))
            v.push_back(i);

    shuffle(v.begin(), v.end(), rng);

    for(int i = 0; i < del_count; i++)
        del_list.insert(v[i]);

    for(int i = 0; i < ant.tours.size(); i++){
        if(del_list.count(i)){
            ant.tours[i].tour = {DEPOT};
            ant.tours[i].cost = 0;
        }else{
            ant.tours[i].tour.pop_back();
            ant.tours[i].cost = ant.tour_length(ant.tours[i]);
        }
    }

    ant.del = del_list;

    return ant;
}

void Ant::verify(int n){
    unordered_set<int> s;
    for(auto &tour : tours){
        for(int i = 1; i < tour.size() - 1; i++){
            if(s.count(tour[i])) assert(false);
            s.insert(tour[i]);
        }
        // if(abs(tour.cost - tour_length(tour)) > 1e-3){
        //     cout << tour.cost << ' ' << tour_length(tour) << endl;
        // }
        assert(abs(tour.cost - tour_length(tour)) < 1);

        assert(tour.tour.front() == 0);
        assert(tour.tour.back() == 0);
    }
    assert(s.size() == n - 1);


}