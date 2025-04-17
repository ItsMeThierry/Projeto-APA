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

    file.close();
}

void escrever_output(solucao sol, int num_pistas, std::string name){
    std::ofstream file("output/"+ name + ".txt");

    if(!file.is_open()){
        throw std::string("[Erro] Nao foi possivel abrir o dados.txt");
    }

    file << sol.multa << std::endl;

    for(int i = 0; i < num_pistas; i++){
        for(voo v : sol.pistas[i]){
            file << v.id << " ";
        }
        file << std::endl;
    }

    file.close();
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
        ler_arquivo("input/arq.txt", dados);
    } catch (int e){
        return e;
    }

    // print_dados(dados);

    voo voos[dados.num_voos];

    voo voos_1[dados.num_voos];

    for(int i = 0; i < dados.num_voos; i++){
        voos[i].t_decolagem = dados.r[i];
        voos[i].duracao = dados.c[i];
        voos[i].multa = dados.p[i];
        voos[i].id = i+1;

        voos_1[i].t_decolagem = voos[i].t_decolagem;
        voos_1[i].duracao = voos[i].duracao;
        voos_1[i].multa = voos[i].multa;
        voos_1[i].id = voos[i].id;
    }

    solucao sol;

    algoritmo_guloso(sol, voos_1, dados.matrix, dados.num_voos, dados.num_pistas);

    escrever_output(sol, dados.num_pistas, "guloso");

    solucao sol_2 = two_opt(sol, dados.matrix, dados.num_pistas);

    escrever_output(sol_2, dados.num_pistas, "opt");

    return 0;
}