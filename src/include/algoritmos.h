#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "structures.h"
#include <climits>
#include <vector>
#include <list>
#include <iostream>

void algoritmo_guloso(solucao &sol, voo* voos, int** matrix, int num_voos, int num_pistas);

int calcula_multa_pista(std::vector<voo> &pista, int**matrix);

void two_opt(solucao &sol, int**matrix, int num_pistas);
void re_insertion(solucao &sol, int**matrix, int num_pistas);
void swap(solucao &sol, int**matrix, int num_pistas);
void swap_pistas(solucao &sol, int**matrix, int num_pistas);

solucao vnd(solucao otimo, int**matrix, int num_pistas);

// solucao ils(solucao &sol, voo* voos, int** matrix, int num_voos, int num_pistas);

#endif