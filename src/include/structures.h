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
    std::vector<Voo> *pistas;
    std::vector<int> multa_pistas;
    int num_pistas;

    Solucao(const int num_pistas) : multa(0), pistas(new std::vector<Voo>[num_pistas]()), multa_pistas(std::vector<int>(num_pistas, 0)), num_pistas(num_pistas){}

    Solucao(const Solucao& other) : multa(other.multa), pistas(new std::vector<Voo>[other.num_pistas]()), num_pistas(other.num_pistas){
        for(int i = 0; i < num_pistas; i++){
            pistas[i] = other.pistas[i];
        }
    }

    Solucao& operator=(const Solucao& other) {
        if (this != &other) {
            num_pistas = other.num_pistas;
            multa = other.multa;

            delete[] pistas;
            pistas = new std::vector<Voo>[num_pistas]();

            for(int i = 0; i < num_pistas; i++){
                pistas[i] = other.pistas[i];
            }
        }
        return *this;
    }

    ~Solucao(){
        delete[] pistas;
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