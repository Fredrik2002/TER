#ifndef GLOUTON_H
#define GLOUTON_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include "backtracking.h"
#include <bits/stdc++.h>

using namespace std;

double glouton1(int N, vector<Arete*> &aretes, int x0){//Liste d'arêtes TRIEE
    vector<Arete*>* solution = new vector<Arete*>;
    unordered_set<int> sommet_explores;
    sommet_explores.insert(x0);
    int sommet_actuel = x0;
    while(solution->size()!=N-1){
        for(int i=0;i<aretes.size();i++){
            Arete* a = aretes.at(i);
            if(a->sommet1==sommet_actuel && sommet_explores.find(a->sommet2)==sommet_explores.end()){
                sommet_actuel = a->sommet2;
                sommet_explores.insert(sommet_actuel);
                solution->push_back(a);
                break;
            }
            if(a->sommet2==sommet_actuel && sommet_explores.find(a->sommet1)==sommet_explores.end()){
                sommet_actuel = a->sommet1;
                sommet_explores.insert(sommet_actuel);
                solution->push_back(a);
                break;
            }
        }
    }
    for(int i=0;i<aretes.size();i++){
        Arete* a = aretes.at(i);
        if(a->sommet1==x0 && a->sommet2==sommet_actuel){
            solution->push_back(a);
            return valeur_solution(*solution);
        }
        if(a->sommet2==x0 && a->sommet1==sommet_actuel){
            solution->push_back(a);
            return valeur_solution(*solution);
        }
    }
}

double glouton2(int N, vector<Arete*> &aretes){
    double solution = glouton1(N, aretes, 0);
    for(int x0=1;x0<N;x0++){
        double s = glouton1(N, aretes, x0);
        if (s < solution){
            solution = s;
        }
    }
    return solution;
}

#endif