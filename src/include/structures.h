#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct Voo{
    int t_decolagem;
    int duracao;
    int multa;
    int id;
} voo;

typedef struct Solucao{
    int multa;
    std::vector<voo> *pistas;
    int num_pistas;

    Solucao(const int num_pistas) : multa(0), pistas(new std::vector<voo>[num_pistas]()), num_pistas(num_pistas){}

    Solucao(const Solucao& other) : multa(other.multa), pistas(new std::vector<voo>[other.num_pistas]()), num_pistas(other.num_pistas){
        for(int i = 0; i < num_pistas; i++){
            pistas[i] = other.pistas[i];
        }
    }

    Solucao& operator=(const Solucao& other) {
        if (this != &other) {
            num_pistas = other.num_pistas;
            multa = other.multa;

            delete[] pistas;
            pistas = new std::vector<voo>[num_pistas]();

            for(int i = 0; i < num_pistas; i++){
                pistas[i] = other.pistas[i];
            }
        }
        return *this;
    }

    ~Solucao(){
        delete[] pistas;
    }
} solucao;

#endif