#ifndef CHRISTOFIDES_H
#define CHRISTOFIDES_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "branch_and_bound.h"
#include "2-approx.h"
#include <bits/stdc++.h>
#include "backtracking.h"
#include "library/Graph.cpp"
#include "library/Matching.cpp"
#include "library/BinaryHeap.cpp"
#include "library/Example.cpp"

using namespace std;

vector<int>* sommet_impairs(int N, vector<Arete*> &ACPM){
    int* degres = new int[N]();
    vector<int>* sommets = new vector<int>();
    for(Arete* a : ACPM){
        degres[a->sommet1] += 1;
        degres[a->sommet2] += 1;
    }
    for(int i=0;i<N;i++){
        if(degres[i]%2==1) sommets->push_back(i);
    }
    return sommets;
}

vector<Arete*> * couplage_parfait(int N, vector<int> &sommets, double* distances){
    ofstream my_file;
    my_file.open("input.txt");
    my_file << sommets.size() << "\n";
    my_file << sommets.size()*(sommets.size()-1)/2 << "\n";
    for(int i=0;i<sommets.size();i++){
        for(int j=i+1;j<sommets.size();j++){
            my_file << i << " " << j << " " << distances[sommets.at(i)*N+sommets.at(j)] << "\n";
        }
    }
    my_file.close();
    vector<int>* matching = MinimumCostPerfectMatchingExample("input.txt");
    vector<Arete*> * aretes_du_matching = new vector<Arete*>();
    for(int i=0;i<matching->size();i+=2){
        aretes_du_matching->push_back(new Arete(sommets.at(matching->at(i)), 
        sommets.at(matching->at(i+1)), distances[sommets.at(matching->at(i))*N+sommets.at(matching->at(i+1))], 1));
    }
    return aretes_du_matching; 
}

int sommet_a_utiliser(vector<Arete*> &ACPM, vector<int> &circuit){
    for(int i=0;i<circuit.size();i++){
        int s = circuit.at(i);
        for(Arete *a : ACPM){
            if(a->sommet1 ==s || a->sommet2 == s) return s;
        }
    }
    return -1;
}

vector<int> circuit_eulerien(vector<Arete*> &ACPM){
    vector<int> circuit, boucle;
    boucle.push_back(ACPM.at(0)->sommet1);
    int sommet_actuel = ACPM.at(0)->sommet1;
    while(true){
        if(sommet_actuel==boucle.at(0) && boucle.size()>1){
            auto it = find(circuit.begin(), circuit.end(), boucle.at(0));
            if(it==circuit.end()) circuit.insert(circuit.begin(), boucle.begin(), boucle.end());
            else circuit.insert(it, boucle.begin(), boucle.end()-1);
            if(ACPM.size()==0) break;
            sommet_actuel = sommet_a_utiliser(ACPM, circuit);
            boucle.clear();
            boucle.push_back(sommet_actuel);
            continue;
        }
        for(int i=0;i<ACPM.size();i++){
            if(ACPM.at(i)->sommet1==sommet_actuel){
                boucle.push_back(ACPM.at(i)->sommet2);
                sommet_actuel=ACPM.at(i)->sommet2;
                ACPM.erase(ACPM.begin()+i);
                break;
            }

            if(ACPM.at(i)->sommet2==sommet_actuel){
                boucle.push_back(ACPM.at(i)->sommet1);
                sommet_actuel=ACPM.at(i)->sommet1;
                ACPM.erase(ACPM.begin()+i);
                break;
            }
        }
    }
    return circuit;
}

void shortcut(int N, vector<int> &circuit){
    unordered_set<int> sommets_explores;
    int i=0;
    while(sommets_explores.size()!=N){
        if(sommets_explores.find(circuit.at(i))==sommets_explores.end()){
            sommets_explores.insert(circuit.at(i));
            i++;
        }
        else{
            circuit.erase(circuit.begin()+i);
        }
    }
    circuit.erase(circuit.begin()+N, circuit.end()-1);
}

double christofides(int N, vector<Arete*> &aretes){
    double* distances = matrice_distance(N, aretes);
    vector<Arete*> * solution = new vector<Arete*>();
    vector<Arete*> ACPM = kruskal(N+1, aretes, -1);
    vector<int>* sommets = new vector<int>();
    sommets = sommet_impairs(N, ACPM);
    vector<Arete*>* matching = couplage_parfait(N, *sommets, distances);
    for(Arete* a: *matching){
        ACPM.push_back(a);
    }
    vector<int> circ = circuit_eulerien(ACPM);
    shortcut(N, circ);
    for(int i=0;i<N;i++){
        solution->push_back(new Arete(circ.at(i), circ.at(i+1), distances[circ.at(i)*N+circ.at(i+1)], 1));
    }
    return valeur_solution(*solution);
}

#endif