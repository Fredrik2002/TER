#include <iostream>
#include "branch_and_bound2.h"

using namespace std;

double distance_de_manhattan(int xi, int xj, int yi, int yj){
    //Retourne la distance de Manhattan entre les points (xi, yi) et (xj, yj)
    return abs(xi-xj)+abs(yi-yj);
}

double distance_euclidienne(int xi, int xj, int yi, int yj){
    //Retourne la distance euclidienne entre les points (xi, yi) et (xj, yj)
    return sqrt(pow((xi-xj), 2)+pow((yi-yj), 2));
}

vector<Arete*> genere_instances(int N, int x_max, int y_max, 
double (*distance)(int, int, int, int)){ 
    /*
    Entrée :
        - N : nombres de sommets
        - x_max, y_max : Coordonnées maximales des points que l'on génère
        - *distance : fonction qui mesure la distance entre 2 points
    Sortie :
        - Liste d'arêtes entre tous les sommets avec leurs poids 
    A noter que les sommets sont numérotés de 0 à N-1.
    */
    int* X = new int[N];
    int* Y = new int[N];
    for(int i=0;i<N;i++){
        
        X[i] = rand()%x_max;
        Y[i] = rand()%y_max;
    }
    vector<Arete*> aretes;
    int p=0;
    for(int i=0;i<N;i++){
        for(int j=i+1;j<N;j++){
            Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]), p);
            aretes.push_back(a);
            p++;
        }
    }
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    return aretes;
}


int main(){
    vector<Arete*> aretes = genere_instances(12, 100, 100, distance_de_manhattan);
    double* matrice = matrice_distance(12, aretes);
    vector<Noeud2> noeuds;
    Noeud2::N = 12;
    Noeud2::m = 6*11;
    Noeud2::distances = matrice;
    for(int i = 0; i < 10000000; i++){
        Noeud2 n;
        noeuds.push_back(n);
    }
    int x;
    cin >> x;

    for(int i = 0; i < 10000000; i++){
        noeuds.at(i).destructeur();
    }
    cin >> x;

}
    
