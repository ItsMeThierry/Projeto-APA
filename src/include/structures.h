#include <vector>

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
    int *voos_multa;
} solucao;

#endif