#include <iostream>
#include <fstream>
#include "include/algoritmos.h"
#include "include/structures.h"

struct Dados{
    int num_voos;
    int num_pistas;

    int* r;
    int* c;
    int* p;

    voo* voos;
    int** matrix;
};

void ler_arquivo(std::string arq, Dados &dados){
    std::ifstream file(arq);

    if(!file.is_open()){
        std::cerr << "[ERRO] Nao foi possivel abrir o arquivo!" << std::endl;
        throw -1;
    }

    if(!(file >> dados.num_voos)){
        std::cerr << "[ERRO] Nao foi possivel ler o numero de voos." << std::endl;
        throw -1;
    }
    if(!(file >> dados.num_pistas)){
        std::cerr << "[ERRO] Nao foi possivel ler o numero de pistas." << std::endl;
        throw -1;
    }

    dados.r = new int[dados.num_voos];
    for(int i = 0; i < dados.num_voos; i++){
        if(!(file >> dados.r[i])){
            std::cerr << "[ERRO] Array r esta com formatacao errada." << std::endl;
            throw -1;
        }
    }

    dados.c = new int[dados.num_voos];
    for(int i = 0; i < dados.num_voos; i++){
        if(!(file >> dados.c[i])){
            std::cerr << "[ERRO] Array c esta com formatacao errada." << std::endl;
            throw -1;
        }
    }

    dados.p = new int[dados.num_voos];
    for(int i = 0; i < dados.num_voos; i++){
        if(!(file >> dados.p[i])){
            std::cerr << "[ERRO] Array p esta com formatacao errada." << std::endl;
            throw -1;
        }
    }

    dados.matrix = new int*[dados.num_voos];
    for(int i = 0; i < dados.num_voos; i++){
        dados.matrix[i] = new int[dados.num_voos];
        for(int j = 0; j < dados.num_voos; j++){
            if(!(file >> dados.matrix[i][j])){
                std::cerr << "[ERRO] Matriz esta com formatacao errada." << std::endl;
                throw -1;
            }
        }
    }

    if(!file.eof()){
        std::cerr << "[ERRO] O arquivo esta com alguma formatacao errada." << std::endl;
        throw -1;
    }
}

void print_dados(Dados &dados){
    std::cout << dados.num_voos << std::endl;
    std::cout << dados.num_pistas << std::endl;
    
    for(int i = 0; i < dados.num_voos; i++){
        std::cout << dados.r[i] << " ";
    }

    std::cout << std::endl;
    for(int i = 0; i < dados.num_voos; i++){
        std::cout << dados.c[i] << " ";
    }

    std::cout << std::endl;
    for(int i = 0; i < dados.num_voos; i++){
        std::cout << dados.p[i] << " ";
    }

    std::cout << std::endl;
    std::cout << std::endl;

    for(int i = 0; i < dados.num_voos; i++){
        for(int j = 0; j < dados.num_voos; j++){
            std::cout << dados.matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(){
    
    Dados dados; // Os dados do arquivo de entrada estarão nessa variavel

    try{
        ler_arquivo("input/inst_1.txt", dados);
    } catch (int e){
        return e;
    }

    voo voos[dados.num_voos];

    for(int i = 0; i < dados.num_voos; i++){
        voos[i].t_decolagem = dados.r[i];
        voos[i].duracao = dados.c[i];
        voos[i].multa = dados.p[i];
        voos[i].id = i+1;
    }

    // Insertion sort, organizando o vetor de voos por ordem crescente de decolagem
    for(int i = 1; i < dados.num_voos; i++){
        for (int j = i; j > 0; j--){
            if(voos[j-1].t_decolagem > voos[j].t_decolagem){
                voo aux = voos[j-1];
                voos[j-1] = voos[j];
                voos[j] = aux;
            }
        }
    }

    algoritmo_1(voos, dados.matrix, dados.num_voos, dados.num_pistas);
}