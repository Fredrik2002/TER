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
        static int* two_lightest;
        static int N;// Nombre de sommets
        static int m;// Nombre d'aretes
        int p;
        int* solution;
        int* sommets_places; // liste des sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        double sommet_actuel;

    Noeud2(){
        solution_realisable = false;
        p = 0;
        solution = new int[N];
        sommets_places = new int[N];
        solution[0] = 0; //On part du sommet 0
        for(int i=0;i<N;i++) sommets_places[i]=-1;
        sommets_places[0]=0;
        evaluation = 0;
        for(int j=0;j<N;j++){
            evaluation += distances[j*N+two_lightest[j*2]]+ distances[j*N+two_lightest[j*2+1]];
        } 
        evaluation /= 2;
    }

    Noeud2(Noeud2 &n, int a){
        solution_realisable = false;
        p = n.p+1;
        evaluation = n.evaluation;
        sommet_actuel = n.sommet_actuel;
        solution = new int[N];
        copy(n.solution, n.solution+N, solution);
        solution[p]=a;
        sommets_places = new int[N];
        copy(n.sommets_places, n.sommets_places+N, sommets_places);
        sommets_places[a]=p;
        evalue();
    }

    ~Noeud2(){
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

    int missing_vertex(){
        for(int i=0;i<N;i++){
            if(sommets_places[i]==-1) return i;
        }
        return -1;
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
        if(p==N-2){
            int e = missing_vertex();
            solution_realisable = true;
            solution[p+1]=e;
            evaluation = calcule_solution_realisable();
        }
        else if(p==1){
            double somme = 0;
            int a = solution[1];
            somme -= distances[two_lightest[0]]+distances[two_lightest[1]]
            +distances[a*N+two_lightest[a*2]]+distances[a*N+two_lightest[a*2+1]];

            if(a==two_lightest[0]) somme +=distances[two_lightest[0]] + distances[two_lightest[1]];
            else somme +=distances[two_lightest[0]] + distances[a];

            if(0==two_lightest[2*a]) sommet_actuel = distances[a*N+two_lightest[2*a]] + distances[a*N+two_lightest[2*a+1]];
            else sommet_actuel =distances[two_lightest[2*a]] + distances[solution[1]];

            evaluation += (somme+sommet_actuel)/2;
        }
        else{
            int a = solution[p];
            double somme = -distances[a*N+two_lightest[a*2]]-distances[a*N+two_lightest[a*2+1]]-sommet_actuel;
            somme += distances[solution[p-2]*N+solution[p-1]] + distances[solution[p-1]*N+a];
            if(two_lightest[a*2]==solution[p-1]) sommet_actuel = distances[a*N+two_lightest[2*a]] + distances[a*N+two_lightest[2*a+1]];
            else sommet_actuel = distances[a*N+two_lightest[2*a]] + distances[solution[p-1]*N+a];
            evaluation += (somme+sommet_actuel)/2;
        }
    }
};

int* lightest_two_weights(int N, double* distances){
    int* weights = new int[2*N];
    for(int s=0;s<N;s++){
        int a = -1;
        int b = -1;
        for(int i=0;i<N;i++){
            if(i!=s){
                if(a==-1) a = i;
                else if(b==-1) b = i;
                else if(distances[s*N+a]>distances[s*N+i]) {
                    if(distances[s*N+a]>distances[s*N+b])a=i;
                    else b=i;
                }
                else if(distances[s*N+b]>distances[s*N+i]) b=i;
            }
        }
        if(distances[s*N+a]<distances[s*N+b]){
            weights[2*s] = a;
            weights[2*s+1] = b;
        }
        else{
            weights[2*s+1] = a;
            weights[2*s] = b;
        }
    }
        return weights;
}

bool comparateur_pointeur_noeud(const Noeud2* a, const Noeud2* b){
    return a->evaluation < b-> evaluation;
}

void branch_and_bound2(Noeud2* &n, int &N, double* &distances, double &borne_sup, int &nb_noeuds_explores){
    vector<Noeud2*> liste_noeuds;
    for(int i=0;i<N;i++){
        if(n->sommets_places[i]==-1 && !(n->sommets_places[1]==-1 && i==2)){
            Noeud2* n_fils = new Noeud2(*n, i);
            nb_noeuds_explores++;
            if(n_fils->evaluation < borne_sup){
                if(n_fils->solution_realisable){
                    borne_sup=n_fils->evaluation;
                }
                auto it = lower_bound(liste_noeuds.begin(), liste_noeuds.end(), n_fils, comparateur_pointeur_noeud);
                liste_noeuds.insert(it, n_fils);
            }
            else {delete n_fils;}
        }
    }
    for(int i=0;i<liste_noeuds.size();i++){
        branch_and_bound2(liste_noeuds.at(i), N, distances, borne_sup, nb_noeuds_explores);
        delete liste_noeuds.at(i);
    } 
}

tuple<double, int> lance_profondeur2(int N, double* &distances, double borne_sup=123456798){
    int nb_noeuds_explores = 1;
    Noeud2* n = new Noeud2();
    branch_and_bound2(n, N, distances, borne_sup, nb_noeuds_explores);
    delete n;
    return make_tuple(borne_sup, nb_noeuds_explores);
}

void branch_and_bound3(Noeud2* &n, int &N, double* &distances, double &borne_sup, int &nb_noeuds_explores){
    vector<Noeud2> liste_noeuds;
    for(int i=0;i<N;i++){
        if(n->sommets_places[i]==-1 && !(n->sommets_places[1]==-1 && i==2)){
            Noeud2* n_fils = new Noeud2(*n, i);
            nb_noeuds_explores++;
            if(n_fils->evaluation < borne_sup){
                if(n_fils->solution_realisable){
                    borne_sup=n_fils->evaluation;
                }
                else{
                    branch_and_bound3(n_fils, N, distances, borne_sup, nb_noeuds_explores);
                }
            }
            delete n_fils;
        }
    }
}

tuple<double, int> lance_profondeur3(int N, double* &distances, double borne_sup=123456798){
    int nb_noeuds_explores = 1;
    Noeud2* n = new Noeud2();
    branch_and_bound3(n, N, distances, borne_sup, nb_noeuds_explores);
    delete n;
    return make_tuple(borne_sup, nb_noeuds_explores);
}

double* Noeud2::distances = nullptr;
int* Noeud2::two_lightest = nullptr;
int Noeud2::N = 0;
int Noeud2::m = 0;

#endif