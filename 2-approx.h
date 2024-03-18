#ifndef DEUX_APPROX
#define DEUX_APPROX

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include "backtracking.h"
#include <bits/stdc++.h>

using namespace std;

vector<Arete*> kruskal(int N, vector<Arete*> &aretes, int x0){//Prend en paramètre la liste TRIEE des arêtes
    //N = le sommet de sommets (en comptant x0)
    int* parent = new int[N];
    vector<Arete*> resultat, solution_vide;
    for(int i=0;i<N;i++){
        parent[i] = i;
    }
    for(int i=0;i<aretes.size();i++){
        Arete* a = aretes.at(i);
        int r1 = trouver_racine(a->sommet1, parent);
        int r2 = trouver_racine(a->sommet2, parent);

        if (r1!=r2 && a->sommet1!=x0 && a->sommet2!=x0){ // On peut prendre l'arête
            parent[r2] = r1; // Lie les composantes connexes
            resultat.push_back(aretes.at(i));
        }
        if(resultat.size()==N-2) break;
    }
    delete parent;
    if(resultat.size()==N-2){
        return resultat;
    }
    else return solution_vide;
}

double deux_approx(int N, vector<Arete*> &aretes){
    vector<Arete*> * solution = new vector<Arete*>();
    vector<Arete*> ACPM = kruskal(N+1, aretes, -1);
    for(Arete* a : ACPM){
        solution->push_back(a);
        solution->push_back(a);
    }
    return valeur_solution(*solution);
}

#endif