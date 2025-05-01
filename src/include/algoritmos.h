#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "structures.h"
#include <climits>
#include <vector>
#include <queue>
#include <math.h>  
#include <random>
#include <chrono>
#include <iostream>

Solucao algoritmo_guloso(const Dados &dados);

// int calcula_multa_pista(const std::vector<Voo> &pista);

// void calcula_tempo_e_multa(const std::vector<Voo> &pista, int &multa, int &t, int pos_1, int pos_2, const Dados& dados)

// Calcula a multa da [pos_1; pos_2)
int calcula_multa_pista(const std::vector<Voo> &pista, int pos_1, int pos_2, const Dados& dados);

// Calcula a multa da [pos_1; pos_2) baseado em uma multa inicial e tempo inicial
void calcula_multa_pista(const std::vector<Voo> &pista, int &multa, int &t, int pos_1, int pos_2, const Dados& dados);

void atualiza_voos(std::vector<Voo> &pista, int pos_alterada, const Dados& dados);

void vnd(Solucao &otimo, const Dados &dados);
void ils(Solucao &s, const Dados &dados);


void swap(Solucao &sol, const Dados &dados);
void swap_pistas(Solucao &sol, const Dados &dados);
void re_insertion(Solucao &sol, const Dados &dados);
void re_insertion_pistas(Solucao &sol, const Dados &dados);
void re_insertion_2(Solucao &sol, const Dados &dados);
void re_insertion_2_pistas(Solucao &sol, const Dados &dados);
void re_insertion_3(Solucao &sol, const Dados &dados);
void re_insertion_3_pistas(Solucao &sol, const Dados &dados);

void rand_swap(Solucao &sol, const Dados &dados);
void rand_swap_pistas(Solucao &sol, const Dados &dados);
void rand_re_insertion(Solucao &sol, const Dados &dados);
void rand_re_insertion_pistas(Solucao &sol, const Dados &dados);
void rand_re_insertion_2(Solucao &sol, const Dados &dados);
void rand_re_insertion_2_pistas(Solucao &sol, const Dados &dados);
void rand_re_insertion_3(Solucao &sol, const Dados &dados);
void rand_re_insertion_3_pistas(Solucao &sol, const Dados &dados);


void sa(Solucao &otimo, const Dados &dados);

long double calcula_temperatura_inicial(Solucao &sol, const Dados &dados);
Solucao generate_neighbor(Solucao sol, const Dados &dados);

Solucao pertubacao(Solucao sol, const Dados &dados);

#endif