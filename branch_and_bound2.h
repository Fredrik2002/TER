#ifndef BRANCH_AND_BOUND2
#define BRANCH_AND_BOUND2

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <tuple>
#include "backtracking.h"

using namespace std;


class Noeud2{
    public:
        double* distances;
        int p;
        int* solution;
        int* sommets_places; // liste des sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        int N, m; // Nombre de sommets, Nombre d'aretes

    Noeud2(double* d, int NB_SOMMETS){
        solution_realisable = false;
        distances = d;
        p = 0;
        N = NB_SOMMETS;
        m=N*(N-1)/2;
        solution = new int[N];
        sommets_places = new int[N];
        solution[0] = 0; //On part du sommet 0
        for(int i=0;i<N;i++) sommets_places[i]=-1;
        sommets_places[0]=0;
        evalue();
    }

    Noeud2(Noeud2 &n, int a){
        solution_realisable = false;
        distances = n.distances;
        N=n.N;
        m=n.m;
        p = n.p+1;
        solution = new int[N];
        copy(n.solution, n.solution+N, solution);
        solution[p]=a;
        sommets_places = new int[N];
        copy(n.sommets_places, n.sommets_places+N, sommets_places);
        sommets_places[a]=p;
        evalue();
    }

    void afficher(){
        cout << "Evaluation du noeud :" << evaluation <<endl;
        cout << "Solution :"<<endl;
        for(int i=0;i<=p;i++){
            cout << solution[i] << " ";
        }
        if(p==N-2) cout << missing_vertex();
        cout <<endl<< "Sommets explores :"<<endl;
        for(int i=0;i<N;i++){
            cout << sommets_places[i] << " ";
        }
        cout << endl;
    }

    bool operator<(const Noeud2 &autre){
        return evaluation<autre.evaluation;
    }

    double lightest_two_weights(int s){
        double a = 100000000;
        double b = 100000000;
        for(int i=0;i<N;i++){
            if(i!=s){
                //cout << distances[s*N+i] << " " << a << " " << b << endl;
                if(a>distances[s*N+i]) {
                    if(a>b)a=distances[s*N+i];
                    else b=distances[s*N+i];
                }
                else if(b>distances[s*N+i]) b=distances[s*N+i];
            }
        }
        return a+b;
    }

    double lightest_weight(int s, int sommet_exclu){
        double a = 100000000;
        for(int i=0;i<N;i++){
            if(i!=s && i!=sommet_exclu){
                if(a>distances[s*N+i]) a=distances[s*N+i];
            }
        }
        return a+distances[s*N+sommet_exclu];
    }

    int missing_vertex(){
        for(int i=0;i<N;i++){
            if(sommets_places[i]==-1) return i;
        }
    }

    double calcule_solution_realisable(){
        double somm = 0;
        for(int i=0;i<N;i++){
            int a = solution[i];
            int b = solution[(i+1)%N];
            somm += distances[a*N+b];
        }
        return somm;
    }

    void evalue() {
        double somme = 0;
        if(p==N-2){
            int e = missing_vertex();
            solution_realisable = true;
            solution[p+1]=e;
            evaluation = calcule_solution_realisable();
        }
        else{
            for(int i=0;i<N;i++){
                if(sommets_places[i]==-1 || p==0) somme +=lightest_two_weights(i);
                else if(sommets_places[i]==p) somme += lightest_weight(i, solution[p-1]);
                else if(sommets_places[i]==0) somme += lightest_weight(i, solution[1]);
                else{
                    int j=sommets_places[i];//indice du sommet i dans solution
                    somme+=distances[i*N+solution[j-1]]+distances[i*N+solution[j+1]];
                }
            }
            evaluation = ceil(somme/2);
        }
        
    }
};

Noeud2 selection_noeud(vector<Noeud2> &liste){ // Strategie de parcours, voir fonction de tri des noeuds
    Noeud2 n = liste.at(0);
    liste.erase(liste.begin());
    return n;
}

void insertion_dichotomique(vector<Noeud2> &liste, Noeud2 &n){
    auto it = lower_bound(liste.begin(), liste.end(), n);
    liste.insert(it, n);
}

tuple<int, int> branch_and_bound2(int N, double* distances, double borne_sup=13245678){
    int nb_noeuds_explores = 0;
    vector<Noeud2> liste_noeuds;
    Noeud2 n(distances, N);
    liste_noeuds.push_back(n);
    while(liste_noeuds.size()>0){
        n = selection_noeud(liste_noeuds);
        nb_noeuds_explores++;
        if(n.solution_realisable){
            cout << "Nb noeuds " << nb_noeuds_explores <<endl;
            return make_tuple(n.evaluation, nb_noeuds_explores);
        }
        else{
            for(int i=0;i<N;i++){
                if(n.sommets_places[i]==-1 && !(n.sommets_places[1]==-1 && i==2)){
                    Noeud2 n_fils(n, i);
                    if(n_fils.evaluation < borne_sup){
                        if(n_fils.solution_realisable){
                        borne_sup=n_fils.evaluation;
                        auto it = lower_bound(liste_noeuds.begin(), liste_noeuds.end(), n_fils);
                        liste_noeuds.erase(it, liste_noeuds.end());
                        liste_noeuds.push_back(n_fils);
                        }
                        else{
                            insertion_dichotomique(liste_noeuds,n_fils);
                        }
                    }
                }
            }
        }
    }
}

#endif