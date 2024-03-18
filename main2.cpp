#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <tuple>
#include "branch_and_bound.h"
#include "glouton.h"
#include "backtracking.h"
#include "2-approx.h"
#include <bits/stdc++.h>
#include "christofides.h"
#include "branch_and_bound2.h"
#include "held-karp.h"
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
    ofstream instances;
    instances.open("main2/instances.txt", ios::app);
    int* X = new int[N];
    int* Y = new int[N];
    for(int i=0;i<N;i++){
        
        X[i] = rand()%x_max;
        Y[i] = rand()%y_max;
    }
    vector<Arete*> aretes;
    int p=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(j>i){
                Arete* a = new Arete(i, j, distance(X[i], X[j], Y[i], Y[j]), p);
                aretes.push_back(a);
                p++;
            }
            instances << distance(X[i], X[j], Y[i], Y[j]) << " ";
        }
        instances << endl;
    }
    instances << endl;
    instances.close();
    sort(aretes.begin(), aretes.end(), comparateur_pointeur);
    return aretes;
}

int main(){
    srand(time(NULL));
    
    ofstream my_file_approx, my_file_exacte, instances;

    my_file_approx.open("main2/datas_approx.csv");
    my_file_exacte.open("main2/datas_exacte.csv");
    instances.open("main2/instances.txt");
    my_file_approx << "N, Solution exacte, Solution Gloutonne 1, Solution Gloutonne 2, Solution 2-Approximation, Solution 3/2-Approximation \n";
    my_file_exacte << "N, Temps de résolution Backtracking, Temps de résolution Branch & Bound1 (Orienté arête),"
    "Nombre de noeuds explorés Branch & Bound1, Temps de résolution Branch & Bound2 (Orienté sommet),"
    "Nombre de noeuds explorés Branch & Bound2, Temps de résolution programmation dynamique, Temps de résolution PLNE \n";
    
    for(int N=5;N<=20;N++){
        int m = N*(N-1)/2;
        Noeud2::N = N;
        Noeud2::m = m;
        Arete* aretes2 = new Arete[m];
        for(int i=0;i<100;i++){
            vector<Arete*> aretes = genere_instances(N, 100, 100, distance_de_manhattan);
            double* matrice = matrice_distance(N, aretes);
            Noeud2::distances = matrice;
            for(int i=0;i<m;i++){
                aretes2[i] = *(aretes.at(i));
            }
            // SOLUTIONS APPROCHEES
            double g1 = glouton1(N, aretes, 0);
            double g2 = glouton2(N, aretes);
            double approx1 = deux_approx(N, aretes);
            double approx2 = christofides(N, aretes);

            
            clock_t startTime = clock();
            double backtrck = backtracking(N, aretes);
            double t1 = (double (clock()-startTime))/1000;
            cout << t1 << "s ";
            
            startTime = clock();
            tuple<double, int> couple = lance_profondeur(N, aretes2);
            double t2 = (double (clock()-startTime))/1000;
            double s1 = get<0>(couple);
            int nb_noeuds = get<1>(couple);
            cout << t2 << "s, "<<nb_noeuds<<" noeuds ";
            
            
            startTime = clock();
            tuple<double, int> couple2 = lance_profondeur3(N, matrice, g2);
            double t3 = (double (clock()-startTime))/1000;
            double s2 = get<0>(couple2);
            int nb_noeuds2 = get<1>(couple2);
            cout <<t3 <<"s, "<<nb_noeuds2<<" noeuds "<< endl;
            

            startTime = clock();
            double held_karp = 0;
            double t4 = (double (clock()-startTime))/1000;

            if(abs(s2-s1)>0.001){
                
                for(Arete *a : aretes){
                    a->afficher();
                }
                cout << "Branch & Bound1 :" << s1 <<endl;
                cout << "backtrck :" << backtrck<<endl;
                cout << "Branch & Bound 2 :" << s2<<endl;
            }
            else{
                //my_file_approx<<backtrck<<"," << valeur_solution(*g1) <<","<<valeur_solution(*g2)<<","<< valeur_solution(*approx1)<<","<<valeur_solution(*approx2)<<"\n";
                my_file_exacte <<t1 <<","<< t2<< ","<<nb_noeuds<<",";
                my_file_exacte << t3 <<","<<nb_noeuds2<<","<<t4<<",\n";
            }
            i++;
        }
        delete[] aretes2;
    }
    
    my_file_approx.close();
    my_file_exacte.close();
    cout << "Exited successfully" << endl;
}