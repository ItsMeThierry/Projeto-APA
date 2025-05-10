#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "structures.h"
#include <climits>
#include <vector>
#include <mutex>
#include <barrier>
#include <thread>
#include <math.h>  
#include <random>
#include <chrono>
#include <iostream>

class CaixaPreta{
    public:
        static Dados dados;

        static Solucao algoritmo_guloso();

        // Calcula a multa da [pos_1; pos_2)
        static int calcula_multa_pista(const std::vector<Voo> &pista, int pos_1, int pos_2);

        // Calcula a multa da [pos_1; pos_2) baseado em uma multa inicial e tempo inicial
        static void calcula_multa_pista(const std::vector<Voo> &pista, int &multa, int &t, int pos_1, int pos_2);

        static void atualiza_voos(std::vector<Voo> &pista, int pos_alterada);

        static void vnd(Solucao &otimo);
        static void ils(Solucao &s);
        static void sa(Solucao &otimo);

        static void swap(Solucao &sol);
        static void swap_pistas(Solucao &sol);
        static void re_insertion(Solucao &sol);
        static void re_insertion_pistas(Solucao &sol);
        static void re_insertion_2(Solucao &sol);
        static void re_insertion_2_pistas(Solucao &sol);
        static void re_insertion_3(Solucao &sol);
        static void re_insertion_3_pistas(Solucao &sol);

        static void rand_swap(Solucao &sol);
        static void rand_swap_pistas(Solucao &sol);
        static void rand_re_insertion(Solucao &sol);
        static void rand_re_insertion_pistas(Solucao &sol);
        static void rand_re_insertion_2(Solucao &sol);
        static void rand_re_insertion_2_pistas(Solucao &sol);
        static void rand_re_insertion_3(Solucao &sol);
        static void rand_re_insertion_3_pistas(Solucao &sol);

        static long double calcula_temperatura_inicial(Solucao &sol);
        static Solucao generate_neighbor(Solucao sol);
};

#endif