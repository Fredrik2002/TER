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
        static double* distances;
        static int N;// Nombre de sommets
        static int m;// Nombre d'aretes
        int p;
        int* solution;
        int* sommets_places; // liste des sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM

    Noeud2(){
        solution_realisable = false;
        p = 0;
        solution = new int[N];
        sommets_places = new int[N];
        solution[0] = 0; //On part du sommet 0
        for(int i=0;i<N;i++) sommets_places[i]=-1;
        sommets_places[0]=0;
        evalue();
    }

    Noeud2(Noeud2 &n, int a){
        solution_realisable = false;
        p = n.p+1;
        solution = new int[N];
        copy(n.solution, n.solution+N, solution);
        solution[p]=a;
        sommets_places = new int[N];
        copy(n.sommets_places, n.sommets_places+N, sommets_places);
        sommets_places[a]=p;
        evalue();
    }

    void destructeur(){
        delete[] solution;
        delete[] sommets_places;
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
            evaluation = somme/2;
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

void branch_and_bound2(Noeud2 &n, int &N, double* &distances, double &borne_sup, int &nb_noeuds_explores){
    nb_noeuds_explores++;
    vector<Noeud2> liste_noeuds;
    for(int i=0;i<N;i++){
        if(n.sommets_places[i]==-1 && !(n.sommets_places[1]==-1 && i==2)){
            Noeud2 n_fils(n, i);
            if(n_fils.evaluation < borne_sup){
                if(n_fils.solution_realisable){
                    borne_sup=n_fils.evaluation;
                }
                else{
                    insertion_dichotomique(liste_noeuds,n_fils);
                }
            }
        }
    }
    for(int i=0;i<liste_noeuds.size();i++){
        branch_and_bound2(liste_noeuds.at(i), N, distances, borne_sup, nb_noeuds_explores);
    }
    n.destructeur();
}

tuple<double, int> lance_profondeur2(int N, double* &distances, double borne_sup=123456798){
    int nb_noeuds_explores = 0;
    Noeud2 n;
    branch_and_bound2(n, N, distances, borne_sup, nb_noeuds_explores);
    return make_tuple(borne_sup, nb_noeuds_explores);
}

void branch_and_bound3(Noeud2* &n, int &N, double* &distances, double &borne_sup, int &nb_noeuds_explores){
    nb_noeuds_explores++;
    vector<Noeud2> liste_noeuds;
    for(int i=0;i<N;i++){
        if(n->sommets_places[i]==-1 && !(n->sommets_places[1]==-1 && i==2)){
            Noeud2* n_fils = new Noeud2(*n, i);
            if(n_fils->evaluation < borne_sup){
                if(n_fils->solution_realisable){
                    borne_sup=n_fils->evaluation;
                }
                else{
                    branch_and_bound3(n_fils, N, distances, borne_sup, nb_noeuds_explores);
                }
            }
        }
    }
    n->destructeur();
}

tuple<double, int> lance_profondeur3(int N, double* &distances, double borne_sup=123456798){
    int nb_noeuds_explores = 0;
    Noeud2* n = new Noeud2();
    branch_and_bound3(n, N, distances, borne_sup, nb_noeuds_explores);
    return make_tuple(borne_sup, nb_noeuds_explores);
}

double* Noeud2::distances = nullptr;
int Noeud2::N = 0;
int Noeud2::m = 0;

#endif