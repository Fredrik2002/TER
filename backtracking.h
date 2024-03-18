#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include <bits/stdc++.h>

using namespace std;

double* matrice_distance(int N, vector<Arete*> &aretes){
    double* distances = new double[N*N];
    for(Arete *a : aretes){
        int i=a->sommet1*N+a->sommet2;
        int j=a->sommet2*N+a->sommet1;
        distances[i]=a->poids;
        distances[j]=a->poids;
    }
    return distances;
}

double* matrice_distance(int N, Arete* &aretes){
    double* distances = new double[N*N];
    int m =N*(N-1)/2;
    for(int k=0;k<m;k++){
        Arete a = aretes[k];
        int i=a.sommet1*N+a.sommet2;
        int j=a.sommet2*N+a.sommet1;
        distances[i]=a.poids;
        distances[j]=a.poids;
    }
    return distances;
}

double valeur_solution(vector<Arete*> &solution){
    double somme = 0;
    for(Arete *a : solution){
        somme += a->poids;
    }
    return somme;
}

double valeur_solution(vector<Arete> &solution){
    double somme = 0;
    for(Arete a : solution){
        somme += a.poids;
    }
    return somme;
}

void placer(int p, int N, double* distances, int* solution_actuelle, double score_actuel, 
int* meilleure_solution, double &meilleur_score, bool* contient){
    if(p==N){
        if(score_actuel+distances[solution_actuelle[0]*N+solution_actuelle[N-1]]<meilleur_score){
            for(int j=0;j<N;j++){
                meilleure_solution[j] = solution_actuelle[j];
            }
            meilleur_score = score_actuel+distances[solution_actuelle[0]*N+solution_actuelle[N-1]];
        }
    }
    else{
        int dernier_sommet = solution_actuelle[p-1];
        for(int i=0;i<N;i++){
                if(!contient[i] && score_actuel+distances[i*N+dernier_sommet]<meilleur_score){// Et que la solution est intéressante
                    solution_actuelle[p] = i;
                    contient[i] = true;
                    placer(p+1, N, distances, solution_actuelle, score_actuel+distances[i*N+dernier_sommet], meilleure_solution, meilleur_score, contient);
                    contient[i] = false;
                }
            }
        }
    }

double backtracking(int N, vector<Arete*> &aretes){
    double* distances = matrice_distance(N, aretes);
    vector<Arete*>* s = new vector<Arete*>();
    int solution_finale[N];
    int solution_actuelle[N];
    bool contient[N];
    contient[0]=true;
    for(int i=1;i<N;i++){
        contient[i]=false;
    }
    double meilleur_score = 1000000000;
    double score_actuel = 0;
    solution_actuelle[0]=0;
    placer(1, N, distances, solution_actuelle, score_actuel, solution_finale, meilleur_score, contient);
    for(int i=0;i<N-1;i++){
        s->push_back(new Arete(solution_finale[i], solution_finale[i+1], distances[solution_finale[i]*N+solution_finale[i+1]],1));
    }
    s->push_back(new Arete(solution_finale[0], solution_finale[N-1], distances[solution_finale[0]*N+solution_finale[N-1]], 1));
    return valeur_solution(*s);
}

#endif