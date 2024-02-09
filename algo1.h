#ifndef ALGO1_H
#define ALGO1_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Arete{
    public:
    int sommet1, sommet2;
    double poids;
    Arete(int s1, int s2, double p){
        sommet1 = s1;
        sommet2 = s2;
        poids = p;
    }

    void afficher(){
        cout << sommet1 << "-" << sommet2 << ", Poids: " << poids <<endl;
    }

    bool operator<(const Arete &autre){
        return poids<autre.poids;
    }
};

class Noeud{
    public:
        vector<Arete> aretes; // Listes des arêtes disponibles
        vector<Arete> solution; // ACPM
        int* degres; // Tableau des degrés de chaque sommets
        bool solution_realisable;
        int evaluation; // Poids de l'ACPM
        int N; // Nombre de sommets

    Noeud(vector<Arete> &ar, vector<Arete> &s, int NB_SOMMETS){
        aretes=ar;
        solution=s;
        solution_realisable=false;
        N = NB_SOMMETS;
        degres = new int[N];
        
    }

    void afficher(){
        for(Arete a : solution){
            a.afficher();
        }
    }

    bool operator<(const Noeud &autre){//On prend en priorité la solution réalisable
        //return evalue()<autre.evalue();
        if(solution_realisable!=autre.solution_realisable){
            return solution_realisable;
        }
        else{
            return evaluation<autre.evaluation;
        }
    }

    void evalue() {
        for(int i=0;i<N;i++){
            degres[i]=0;
        }
        for(Arete a : solution){
            degres[a.sommet1]+=1;
            degres[a.sommet2]+=1;
        }
        int sommet = max_element(degres, degres+N)-&degres[0];
        int somme = 0;
        for(Arete a : solution){
            somme+=a.poids;
        }
        evaluation = somme;
        solution_realisable=true;
        for(int i=0;i<N;i++){
            if(degres[i]!=2){
                solution_realisable = false;
                break;
            } 
        }
        
    }
};

void affiche_liste(vector<Arete> &liste){
    for(Arete a : liste){
        a.afficher();
    }
}

int trouver_racine(int sommet, int* parent){
    if(parent[sommet]==sommet) return sommet;
    return trouver_racine(parent[sommet], parent);
}

vector<Arete> kruskal(int N, vector<Arete> &aretes, int x0){//Prend en paramètre la liste TRIEE des arêtes
    int* parent = new int[N+2];
    vector<Arete> resultat, solution_vide;
    for(int i=0;i<N+2;i++){
        parent[i] = i;
    }
    for(int i=0;i<aretes.size();i++){
        Arete a = aretes.at(i);
        int r1 = trouver_racine(a.sommet1, parent);
        int r2 = trouver_racine(a.sommet2, parent);

        if (r1!=r2 && a.sommet1!=x0 && a.sommet2!=x0){ // On peut prendre l'arête
            parent[a.sommet2] = a.sommet1; // Lie les composantes connexes
            resultat.push_back(aretes.at(i));
        }
        if(resultat.size()==N-1) break;
    }
    if(resultat.size()==N-1){
        return resultat;
    }
    else return solution_vide;

}

vector<Arete> calcule_solution(int N, int x0, vector<Arete> aretes){//Passage par copie
    // Stocke les arêtes incidentes à x0, les 2 premières sont donc les plus légères
    vector<Arete> aretes_retirees, solution;
    int i=0;
    while(i<aretes.size()){// On n'a besoin que des 2 arêtes les plus légères
        Arete a = aretes.at(i);
        if(a.sommet1 == x0 || a.sommet2 == x0){
            aretes_retirees.push_back(aretes.at(i));
            aretes.erase(aretes.begin()+i);
        }
        else i++;
        
    }
    if(aretes_retirees.size()>=2){
        solution = kruskal(N-1, aretes, x0);
        solution.push_back(aretes_retirees.at(0));
        solution.push_back(aretes_retirees.at(1));
    }
    
    return solution;
}

vector<Arete> sommet_a_separer(int N, Noeud &n){//Renvoie les arêtes à retirer 
    //(Les arêtes ont toutes un sommet en commun)
    // STRATEGIE DE SEPARATION : A ETUDIER (On prend le sommet de plus haut degré pour l'instant)
    
    int sommet = max_element(n.degres, n.degres+N)-&(n.degres[0]);
    vector<Arete> aretes_a_brancher;
    for(int i=0;i<n.solution.size();i++){
        Arete a = n.solution.at(i);
        if (a.sommet1==sommet || a.sommet2 == sommet) aretes_a_brancher.push_back(n.solution.at(i));
    }
    return aretes_a_brancher;
}

Noeud selection_noeud(vector<Noeud> &liste){ // Strategie de parcours, voir fonction de tri des noeuds
    sort(liste.begin(), liste.end());
    Noeud n = liste.at(0);
    liste.erase(liste.begin());
    return n;
}

vector<Arete> retire_arete(vector<Arete> aretes, Arete a){//Passage par copie OBLIGATOIRE
    for(int i=0;i<aretes.size();i++){
        Arete a1 = aretes.at(i);
        if(a1.sommet1==a.sommet1 && a1.sommet2==a.sommet2){
            aretes.erase(aretes.begin()+i);
            return aretes;
        }
    }
    return aretes;
}

vector<Arete> algorithme1(int N, vector<Arete> &aretes){
    int x0 = 0;
    int nb_noeuds_explores = 0;
    vector<Arete> best_sol; 
    vector<Arete> solution = calcule_solution(N, x0, aretes);
    vector<Noeud> liste_noeuds;
    Noeud n(aretes, solution, N);
    n.evalue();
    int borne_inf = n.evaluation;
    int borne_sup = 21156135;
    liste_noeuds.push_back(n);
    while(liste_noeuds.size()>0){
        cout << "Nb noeuds" << nb_noeuds_explores <<endl;
        n = selection_noeud(liste_noeuds);
        nb_noeuds_explores++;
        if(n.solution_realisable){//Solution réalisable
        
            if(n.evaluation<borne_sup){
                borne_sup=n.evaluation;
                best_sol=n.solution;
            }
            if(n.evaluation==borne_inf){
                return best_sol;
            }
        }
        else if(n.evaluation<borne_sup){
            vector<Arete> branchement = sommet_a_separer(N, n);
            for(Arete a : branchement){
                vector<Arete> new_aretes = retire_arete(n.aretes, a);
                vector<Arete> sol = calcule_solution(N, x0, new_aretes);
                Noeud n_fils(new_aretes, sol, N);
                n_fils.evalue();
                if(n_fils.solution.size()!=0) liste_noeuds.push_back(n_fils); // Pas assez d'arêtes
            }
        }
    }
    cout << "Nb noeuds" << nb_noeuds_explores <<endl;
    return best_sol; // Aucune solution réalisable
}

#endif