#ifndef HELD_KARP_H
#define HELD_KARP_H

#include <vector>
#include <iostream>

using namespace std;


double held_karp(int N, double* &graph, int pos, int visited, vector<vector<double>>& state) {
    if(visited == ((1 << N) - 1))
        return graph[pos*N]; 

    if(state[pos][visited] != 100000)
        return state[pos][visited];

    for(int i = 0; i < N; ++i) {
        if(i == pos || (visited & (1 << i)))
            continue;
        int distance = graph[pos*N+i] + held_karp(N, graph, i, visited | (1 << i), state);
        if(distance < state[pos][visited])
            state[pos][visited] = distance;
    }
    return state[pos][visited];
}

#endif