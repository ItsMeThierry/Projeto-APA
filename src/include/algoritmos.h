#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "structures.h"
#include <climits>
#include <vector>
#include <math.h>  
#include <random>
#include <chrono>
#include <iostream>

solucao algoritmo_guloso(voo* voos, int** matrix, int num_voos, int num_pistas);

int calcula_multa_pista(std::vector<voo> &pista, int**matrix);

void re_insertion(solucao &sol, int**matrix, int num_pistas);
void re_insertion_pistas(solucao &sol, int**matrix, int num_pistas);
void swap(solucao &sol, int**matrix, int num_pistas);
void swap_pistas(solucao &sol, int**matrix, int num_pistas);
void re_insertion_2(solucao &sol, int**matrix, int num_pistas);
void re_insertion_2_pistas(solucao &sol, int**matrix, int num_pistas);


void sa(solucao &otimo, int**matrix, int num_pistas);
long double calcula_temperatura_inicial(solucao &sol, int**matrix, int num_pistas);
solucao generate_neighbor(solucao sol, int**matrix, int num_pistas);


#endif