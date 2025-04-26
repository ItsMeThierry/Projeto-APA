#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "structures.h"
#include <climits>
#include <vector>
#include <cstdlib>
#include <ctime>
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


void rvns(solucao &otimo, int**matrix, int num_pistas);
solucao shake(solucao sol, int k, int**matrix, int num_pistas);

void ils(solucao &s, int** matrix, int num_pistas);
solucao pertubacao(solucao sol, int**matrix, int num_pistas);

#endif