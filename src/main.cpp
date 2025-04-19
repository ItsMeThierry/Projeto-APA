#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
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
        std::cerr << "[ERRO] O arquivo esta com alguma formatacao errada. Verifique se há uma linha extra vazia no final do arquivo." << std::endl;
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
    
    //std::string inst[] = {"n500m10E"};
    //std::string inst[] = {"n500m10E", "n700m12E", "n1000m15E"};
    std::string inst[] = {"n3m10A", "n3m10B", "n3m10C", "n3m10D", "n3m10E", "n3m20A", "n3m20B", "n3m20C", "n3m20D", "n3m20E", "n3m40A", "n3m40B", "n3m40C", "n3m40D", "n3m40E", "n5m50A", "n5m50B", "n5m50C", "n5m50D", "n5m50E"};
    std::vector<int> valores_otimos;
    std::vector<int> valores_heuristicos;

    for(std::string s : inst){
        Dados dados;

        try{
            ler_arquivo("input/instancias/" + s + ".txt", dados);
            //ler_arquivo("input/copa_apa/" + s + ".txt", dados);
        } catch (int e){
            return e;
        }

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

        solucao sol_2 = vnd(sol, dados.matrix, dados.num_pistas);

        escrever_output(sol_2, dados.num_pistas, "swap");


        // for(int i = 0; i < dados.num_pistas; i++){
        //     for(int v = 0, t = 0; v < sol_2.pistas[i].size(); v++){
        //         if(v > 0) t += dados.matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
        //         if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
        //         else if(t > sol.pistas[i][v].t_decolagem){ std::cout << sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem) << '\n'; }
        //         t += sol.pistas[i][v].duracao;
        //     }
        // }

        valores_otimos.push_back(sol.multa);
        valores_heuristicos.push_back(sol_2.multa);


    }

    std::cout << "=====================================================" << std::endl;
    std::cout << "                        VND" << std::endl;
    std::cout << "INSTANCIA\tOTIMO\tVALOR SOLUCAO\tTEMPO\tGAP" << std::endl;
    std::cout << "=====================================================" << std::endl;

    for(uint32_t i = 0; i < valores_otimos.size(); i++){
        float gap = ((float)(valores_otimos[i]-valores_heuristicos[i])/valores_otimos[i])*100.00f;
        std::cout << std::left << std::setw(16) << inst[i]
                  << std::left << std::setw(8) << valores_otimos[i]
                  << std::right << std::setw(13) << valores_heuristicos[i]
                  << std::right << std::setw(8) << "00:00"
                  << std::right << std::setprecision(3) << std::setw(7) << gap << std::endl; 
    }
    

    return 0;
}