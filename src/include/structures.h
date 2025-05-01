#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Voo{
    int id;
    int multa_acumulada;
    int t_decolagem;

    Voo() : id(-1), multa_acumulada(0), t_decolagem(0){}
    Voo(const int id) : id(id), multa_acumulada(0), t_decolagem(0) {}
} Voo;

typedef struct Solucao{
    int multa;
    std::vector<std::vector<Voo>> pistas;
    std::vector<int> multa_pistas; 

    Solucao() : multa(0), pistas(), multa_pistas() {} 
    Solucao(const int num_pistas) :  multa(0), pistas(num_pistas), multa_pistas(num_pistas, 0){}

    Solucao(const Solucao& other) : multa(other.multa), pistas(other.pistas), multa_pistas(other.multa_pistas){}

    Solucao& operator=(const Solucao& other) {
        if (this != &other) {
            multa = other.multa;
            pistas = other.pistas;
            multa_pistas = other.multa_pistas;
        }
        return *this;
    }
    
    
} Solucao;

struct Dados{
    int num_voos;
    int num_pistas;

    int* array_decolagem;
    int* array_duracao;
    int* array_penalidade;

    int** matrix;
};

#endif