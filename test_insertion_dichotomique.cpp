#include <iostream>
#include <vector>
#include "branch_and_bound.h"
#include <ctime>

using namespace std;

void benchmark(int taille_liste){
    vector<Noeud> liste_noeud;
    Arete *aretes;
    int k=10000;
    clock_t startTime;
    int x0 = 0;
    double t;

    for(int i=0;i<taille_liste;i++){
        liste_noeud.push_back(Noeud(aretes, 1, x0));
        liste_noeud.at(i).evaluation = rand()%900+100;
    }

    startTime = clock();
    for(int i=0;i<k;i++){
        Noeud n(aretes, 1, x0);
        n.evaluation = rand()%900+100;
        auto it = lower_bound(liste_noeud.begin(), liste_noeud.end(), n);
        liste_noeud.insert(it, n);
    }
    t = (double(clock()-startTime))/1000;
    cout << "Temps pour " << taille_liste << "elements : "<<t <<"s"<< endl;

}

int main(){
    benchmark(100);
    benchmark(1000);
    benchmark(10000);
    benchmark(100000);
    return 0;
    

}