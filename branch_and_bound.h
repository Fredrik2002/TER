#ifndef BRANCH_AND_BOUND
#define BRANCH_AND_BOUND

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <tuple>

using namespace std;

int trouver_racine(int sommet, int* parent){
    if(parent[sommet]==sommet) return sommet;
    return trouver_racine(parent[sommet], parent);
}

int fonction_de_hachage(int a, int b, int N){
    return -a*(a+3)/2+a*N+b-1;
}



class Arete{
    public:
    int sommet1, sommet2, id;
    double poids;
    Arete(){}
    Arete(int s1, int s2, double p, int i){
        sommet1 = s1;
        sommet2 = s2;
        poids = p;
        id = i;
    }

    void afficher(){
        cout << sommet1 << "-" << sommet2 << ", Poids: " << poids <<" - ID :" << id << endl;
    }

    bool operator<(const Arete &autre){
        return poids<autre.poids;
    }

    int hash(int N){
        return fonction_de_hachage(sommet1, sommet2, N);
    }
};

void affiche_liste(vector<Arete*> &liste){
    for(Arete* a : liste){
        a->afficher();
    }
}

bool comparateur_pointeur(const Arete* a, const Arete* b){
    return a->poids < b-> poids;
}

class Noeud{
    public:
        Arete* aretes; // Listes des arêtes disponibles
        int* solution; // ACPM
        bool* aretes_interdites;
        int* degres; // Tableau des degrés de chaque sommets
        bool solution_realisable;
        double evaluation; // Poids de l'ACPM
        int N, m; // Nombre de sommets, Nombre d'aretes
        string hashcode;
        int id;
        int x0;

    Noeud(Arete* &ar,  int NB_SOMMETS, int &sommet_interdit){
        aretes=ar;
        solution_realisable=false;
        N = NB_SOMMETS;
        m=N*(N-1)/2;
        degres = new int[N]();
        solution = new int[N];
        solution[N-3] = -1;
        aretes_interdites = new bool[m]();
        x0 = sommet_interdit;
        hashcode = string(m, '0'); 
    }

    Noeud(Noeud &n, int a){
        aretes=n.aretes;
        solution_realisable=false;
        N=n.N;
        m = n.m;
        degres = new int[N]();
        solution = new int[N];
        solution[N-3] = -1;
        aretes_interdites = new bool[m]();
        copy(n.aretes_interdites, n.aretes_interdites+m, aretes_interdites);
        aretes_interdites[a] = true;
        hashcode = n.hashcode;
        hashcode[a]='1';
        x0 = n.x0;
        evalue();
    }

    ~Noeud(){
        delete[] degres;
        delete[] solution;
        delete[] aretes_interdites;
    }

    void afficher(){
        cout << "Evaluation du noeud :" << evaluation <<endl;
        cout << "Solution réalisable :"<<solution_realisable<<endl;
        for(int i=0;i<N;i++){
            aretes[solution[i]].afficher();
        }
        cout << "Hashcode :" << hashcode << endl << endl;
    }

    bool operator<(const Noeud &autre){
        return evaluation<autre.evaluation;
    }

    void evalue() {
        calcule_solution();
        if(solution[N-3]==-1) return;
        for(int i=0;i<N;i++){
            Arete a = aretes[solution[i]];
            degres[a.sommet1]+=1;
            degres[a.sommet2]+=1;
        }
        int sommet_max = max_element(degres, degres+N)-&degres[0];
        int sommet_min = min_element(degres, degres+N)-&degres[0];
        double somme = 0;
        for(int i=0;i<N;i++){
            somme+=aretes[solution[i]].poids;
        }
        evaluation = somme;
        if(degres[sommet_max]==2 && degres[sommet_min]==2){ //Connexe + tous les sommets de degré 2 => solution réalisable
            solution_realisable = true;
        }
    }

    void kruskal(){//Prend en paramètre la liste TRIEE des arêtes
        //N = le sommet de sommets (en comptant x0)
        int parent[N];
        int p=0;
        for(int i=0;i<N;i++){
            parent[i] = i;
        }
        for(int i=0;i<m;i++){
            Arete a = aretes[i];
            int r1 = trouver_racine(a.sommet1, parent);
            int r2 = trouver_racine(a.sommet2, parent);

            if (r1!=r2 && a.sommet1!=x0 && a.sommet2!=x0 && aretes_interdites[i]==0){ // On peut prendre l'arête
                parent[r2] = r1; // Lie les composantes connexes
                solution[p] = i;
                p++;
            }
            if(p==N-2) break;
        }
    }

    void calcule_solution(){
        // Stocke les arêtes incidentes à x0, les 2 premières sont donc les plus légères
        vector<int> aretes_retirees;
        int i=0;
        while(i<m && aretes_retirees.size()<2){// On n'a besoin que des 2 arêtes les plus légères
            Arete a = aretes[i];
            if((a.sommet1 == x0 || a.sommet2 == x0) && !aretes_interdites[i]){
                aretes_retirees.push_back(i);
            }
            i++;
        }
        if(aretes_retirees.size()>=2){
            kruskal();
            if(solution[N-3]!=-1){//On a un arbre couvrant
                solution[N-2]=aretes_retirees.at(0);
                solution[N-1]=aretes_retirees.at(1);
            }
        }
    }
};


vector<int> sommet_a_separer(int N, Noeud &n){//Renvoie les arêtes à retirer 
    //(Les arêtes ont toutes un sommet en commun)
    // STRATEGIE DE SEPARATION : A ETUDIER (On prend le sommet de plus haut degré pour l'instant)
    
    int sommet = max_element(n.degres, n.degres+N)-&(n.degres[0]);
    vector<int> aretes_a_brancher;
    for(int i=0;i<N;i++){
        Arete a = n.aretes[n.solution[i]];
        if (a.sommet1==sommet || a.sommet2 == sommet) aretes_a_brancher.push_back(n.solution[i]);
    }
    return aretes_a_brancher;
}

void insertion_dichotomique(vector<Noeud> &liste, Noeud &n){
    auto it = lower_bound(liste.begin(), liste.end(), n);
    liste.insert(it, n);
}

void branch_and_bound_profondeur(Noeud* &n, int N, Arete* &aretes, double &borne_sup, int &nb_noeuds_explores, 
                unordered_set<string> &set){
    nb_noeuds_explores++;
    vector<int> branchement = sommet_a_separer(N, *n);
    for(int a : branchement){
        Noeud* n_fils = new Noeud(*n, a);
        if(n_fils->solution[N-3]!=-1 && set.find(n_fils->hashcode)==set.end() && n_fils->evaluation<borne_sup){
            set.insert(n_fils->hashcode); 
            if(n_fils->solution_realisable){
                borne_sup=n_fils->evaluation;
            }
            else{
                branch_and_bound_profondeur(n_fils, N, aretes, borne_sup, nb_noeuds_explores, set);
            }
        }
        delete n_fils;
    }
}

tuple<double, int> lance_profondeur(int N, Arete* &aretes, double borne_sup=13245678){
    tuple<double, int> to_return;
    int x0 = 0;
    int nb_noeuds_explores = 0;
    unordered_set<string> set;
    borne_sup+=0.0001;
    Noeud* n = new Noeud(aretes, N, x0);
    n->evalue();
    if(n->solution_realisable) borne_sup=n->evaluation;
    branch_and_bound_profondeur(n, N, aretes, borne_sup, nb_noeuds_explores, set);
    return make_tuple(borne_sup, nb_noeuds_explores);
}



#endif