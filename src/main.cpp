#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <functional>
#include <chrono>
#include "include/algoritmos.h"
#include "include/structures.h"

void ler_arquivo(std::string arq){
    std::ifstream file(arq);

    if(!file.is_open()){
        std::cerr << "[ERRO] Nao foi possivel abrir o arquivo " << arq << "!" << std::endl;
        throw -1;
    }

    if(!(file >> CaixaPreta::dados.num_voos)){
        std::cerr << "[ERRO] Nao foi possivel ler o numero de voos no " << arq << "." << std::endl;
        throw -1;
    }
    if(!(file >> CaixaPreta::dados.num_pistas)){
        std::cerr << "[ERRO] Nao foi possivel ler o numero de pistas no " << arq << "." << std::endl;
        throw -1;
    }

    CaixaPreta::dados.array_decolagem = new int[CaixaPreta::dados.num_voos];
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        if(!(file >> CaixaPreta::dados.array_decolagem[i])){
            std::cerr << "[ERRO] Array decolagem esta com formatacao errada no " << arq << "." << std::endl;
            throw -1;
        }
    }

    CaixaPreta::dados.array_duracao = new int[CaixaPreta::dados.num_voos];
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        if(!(file >> CaixaPreta::dados.array_duracao[i])){
            std::cerr << "[ERRO] Array duracao esta com formatacao errada no " << arq << "." << std::endl;
            throw -1;
        }
    }

    CaixaPreta::dados.array_penalidade = new int[CaixaPreta::dados.num_voos];
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        if(!(file >> CaixaPreta::dados.array_penalidade[i])){
            std::cerr << "[ERRO] Array penalidade esta com formatacao errada no " << arq << "." << std::endl;
            throw -1;
        }
    }

    CaixaPreta::dados.matrix = new int*[CaixaPreta::dados.num_voos];
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        CaixaPreta::dados.matrix[i] = new int[CaixaPreta::dados.num_voos];
        for(int j = 0; j < CaixaPreta::dados.num_voos; j++){
            if(!(file >> CaixaPreta::dados.matrix[i][j])){
                std::cerr << "[ERRO] Matriz esta com formatacao errada no " << arq << "." << std::endl;
                throw -1;
            }
        }
    }

    if(!file.eof()){
        std::cerr << "[ERRO] O arquivo esta com alguma formatacao errada. Verifique se há uma linha extra vazia no final do " << arq << "." << std::endl;
        throw -1;
    }

    file.close();
}

Solucao ler_solucao(std::string arq){
    std::ifstream file(arq);

    if(!file.is_open()){
        std::cerr << "[ERRO] Nao foi possivel abrir o arquivo " << arq << "!" << std::endl;
        throw -1;
    }

    Solucao sol(CaixaPreta::dados.num_pistas);
    std::string linha;

    if (getline(file, linha)) {
        std::stringstream ss(linha);
        if (!(ss >> sol.multa)) {
            std::cerr << "[ERRO] Nao ha valor multa no arquivo " << arq << std::endl;
            throw -1;
        }
    } else {
        std::cerr << "[ERRO] Arquivo " << arq << " esta vazio" << std::endl;
        throw -2;
    }

    int fila_id = 0;
    while (getline(file, linha)) {
        std::stringstream ss(linha);
        std::vector<Voo> fila;
        int numero;

        while (ss >> numero) {
            Voo v(numero);
            fila.push_back(v);
        }

        sol.pistas[fila_id] = fila;
        CaixaPreta::atualiza_voos(sol.pistas[fila_id], 0);
        sol.multa_pistas[fila_id] = sol.pistas[fila_id].back().multa_acumulada;
        fila_id++;
    }

    file.close();

    return sol;
}

void escrever_output(Solucao &sol, std::string name){
    std::ofstream file("output/"+ name + ".txt");

    if(!file.is_open()){
        throw std::string("[Erro] Nao foi possivel abrir o dados.txt");
    }

    file << sol.multa << std::endl;

    for(int i = 0; i < CaixaPreta::dados.num_pistas; i++){
        for(Voo v : sol.pistas[i]){
            file << v.id << " ";
        }
        file << std::endl;
    }

    file.close();
}

void print_dados(){
    std::cout << CaixaPreta::dados.num_voos << std::endl;
    std::cout << CaixaPreta::dados.num_pistas << std::endl;
    
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        std::cout << CaixaPreta::dados.array_decolagem[i] << " ";
    }

    std::cout << std::endl;
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        std::cout << CaixaPreta::dados.array_duracao[i] << " ";
    }

    std::cout << std::endl;
    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        std::cout << CaixaPreta::dados.array_penalidade[i] << " ";
    }

    std::cout << std::endl;
    std::cout << std::endl;

    for(int i = 0; i < CaixaPreta::dados.num_voos; i++){
        for(int j = 0; j < CaixaPreta::dados.num_voos; j++){
            std::cout << CaixaPreta::dados.matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void debug_pista(const std::vector<Voo> &pista, std::ofstream &file){
    int multa = 0;
    std::vector<int> multa_acumulada;
    std::vector<int> tempos;

    for(int v = 0, t = 0; v < (int) pista.size(); v++){
        if(v > 0) t += CaixaPreta::dados.matrix[pista[v-1].id - 1][pista[v].id - 1];
        if(t < CaixaPreta::dados.array_decolagem[pista[v].id - 1]){ t += CaixaPreta::dados.array_decolagem[pista[v].id - 1] - t; }
        else if(t > CaixaPreta::dados.array_decolagem[pista[v].id - 1]){ multa += CaixaPreta::dados.array_penalidade[pista[v].id - 1] * (t - CaixaPreta::dados.array_decolagem[pista[v].id - 1]); }
        tempos.push_back(t);
        t += CaixaPreta::dados.array_duracao[pista[v].id - 1];
        multa_acumulada.push_back(multa);
    }
    
    file << "MULTAS: ";
    for(int m : multa_acumulada){
        file << m << " ";
    }

    file << std::endl;

    file << "TEMPOS: ";
    for(int t : tempos){
        file << t << " ";
    }

    file << std::endl;
    file << std::endl;
}

void debug_solucao(const Solucao &sol, std::string name){
    std::ofstream file("debug/"+ name + ".txt");

    if(!file.is_open()){
        throw std::string("[Erro] Nao foi possivel abrir a pasta debug");
    }

    for(int i = 0; i < CaixaPreta::dados.num_pistas; i++){
        file << "Pista " << i+1 << std::endl;
        file << "Multa total: " << sol.multa_pistas[i] << std::endl;

        file << "ID: ";
        for(Voo v : sol.pistas[i]){
            file << v.id << " ";
        }

        file << std::endl;
        file << "multa_acumulada: ";
        for(Voo v : sol.pistas[i]){
            file << v.multa_acumulada << " ";
        }
        file << std::endl;

        file << "t_decolagem: ";
        for(Voo v : sol.pistas[i]){
            file << v.t_decolagem << " ";
        }
        file << std::endl;
        file << std::endl;
    }
    file << std::endl;
    file << std::endl;

    for(int i = 0; i < CaixaPreta::dados.num_pistas; i++){
        debug_pista(sol.pistas[i], file);
    }

    file.close();
}

int main(){

    std::vector<std::string> inst;

    std::string input;

    std::cout << "Cole o nome dos arquivos" << std::endl;
    getline(std::cin, input);
    std::stringstream ss(input);
    std::string arq;

    while (ss >> arq) {
        inst.push_back(arq);
    }

    std::string opcao;

    std::cout << "Vai usar o ILS ou SA? " << std::endl;
    getline(std::cin, opcao);

    if (opcao != "ILS" && opcao != "ils" && opcao != "SA" && opcao != "sa"){
        std::cout << "Opcao invalida" << std::endl;
        return -1;
    }

    std::string resposta;

    std::cout << "Já existe uma solucao para cada instancia? (s/n)" << std::endl;
    getline(std::cin, resposta);  
    
    if (resposta != "s" && resposta != "S" && resposta != "n" && resposta != "N"){
        std::cout << "Resposta invalida" << std::endl;
        return -1;
    }

    std::vector<int> valores_guloso;
    std::vector<int> valores;
    std::vector<std::chrono::microseconds> tempo;
    
    for(std::string s : inst){
        Solucao sol;

        try{
            ler_arquivo("input/" + s + ".txt");
            
            if(resposta == "s" || resposta == "S"){
                sol = ler_solucao("input/resultados/" + s + ".txt");
            }
        } catch (int e){
            return e;
        }

        sol = CaixaPreta::algoritmo_guloso();
        valores_guloso.push_back(sol.multa);

        CaixaPreta::vnd(sol);

        auto inicio = std::chrono::high_resolution_clock::now();
        if(opcao == "ILS" || opcao == "ils"){
            CaixaPreta::ils(sol);
        } else if(opcao == "SA" || opcao == "sa"){
            CaixaPreta::sa(sol);
        }
        auto fim = std::chrono::high_resolution_clock::now();
        auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);

        valores.push_back(sol.multa);
        tempo.push_back(duracao);
        
        escrever_output(sol, s + "_" + opcao);
    }

    std::cout << "=============================================================" << std::endl;
    std::cout << "                             ILS+SA" << std::endl;
    std::cout << "INSTANCIA\tOTIMO\tVALOR Solucao\tTEMPO (μs)\tGAP" << std::endl;
    std::cout << "=============================================================" << std::endl;

    for(uint32_t i = 0; i < valores_guloso.size(); i++){
        float gap = ((float)(valores_guloso[i]-valores[i])/valores_guloso[i])*100.00f;
        std::cout << std::left << std::setw(16) << inst[i]
                  << std::left << std::setw(8) << valores_guloso[i]
                  << std::right << std::setw(13) << valores[i]
                  << std::right << std::setw(13) << tempo[i].count()
                  << std::right << std::setprecision(3) << std::setw(9) << gap << std::endl; 
    }

    std::cout << std::endl;

    float media = 0;
    for(uint32_t i = 0; i < valores_guloso.size(); i++){
        media += ((float)(valores_guloso[i]-valores[i])/valores_guloso[i])*100.00f;
    }

    std::cout << media/valores_guloso.size() << std::endl;

    return 0;
}