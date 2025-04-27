#include "include/algoritmos.h"

static std::random_device rd;
static std::mt19937 gen(rd());

solucao algoritmo_guloso(voo* voos, int** matrix, int num_voos, int num_pistas){
    solucao sol(num_pistas);
    
    // Insertion sort, organizando o vetor de voos por ordem crescente de decolagem
    for(int i = 1; i < num_voos; i++){
        for (int j = i; j > 0; j--){
            if(voos[j-1].t_decolagem > voos[j].t_decolagem){
                voo aux = voos[j-1];
                voos[j-1] = voos[j];
                voos[j] = aux;
            }

            if(voos[j-1].t_decolagem == voos[j].t_decolagem){
                if(voos[j-1].multa < voos[j].multa){
                    voo aux = voos[j-1];
                    voos[j-1] = voos[j];
                    voos[j] = aux;
                }
            }
        }
    }

    int t_pistas[num_pistas]; // Representa o tempo da pista
    int multa = 0; // Valor da multa

    for (int i = 0; i < num_pistas; i++){
        t_pistas[i] = 0;
    }

    for(int i = 0; i < num_voos; i++){
        int menor_multa = INT_MAX;
        int pista_inserida = -1;
        int t = 0;

        for(int j = 0; j < num_pistas; j++){
            int t_espera = 0;
            int t_vazio = 0;
            int m = 0;

            if(!sol.pistas[j].empty()){ // Se um voo já decolou antes, procura o tempo de espera entre o voo anterior e esse de agora
                t_espera = matrix[sol.pistas[j].back().id - 1][voos[i].id - 1];
            }

            if(t_pistas[j] + t_espera < voos[i].t_decolagem){ // Se o voo de agora vai demorar para decolar, acrescenta o tempo de pista vazia
                t_vazio = voos[i].t_decolagem - (t_pistas[j] + t_espera);
            } else if(t_pistas[j] + t_espera > voos[i].t_decolagem){
                m = voos[i].multa * (t_pistas[j] + t_espera - voos[i].t_decolagem);
            }

            if(m < menor_multa){
                menor_multa = m;
                pista_inserida = j;
                t = t_espera + t_vazio;
            }
        }

        sol.pistas[pista_inserida].push_back(voos[i]);
        t_pistas[pista_inserida] += t + voos[i].duracao;
        multa += menor_multa;
    }

    sol.multa = multa;

    return sol;
}

int calcula_multa_pista(std::vector<voo> &pista, int** matrix){
    int multa = 0;
    int size = pista.size();
    for(int  i = 0, t = 0; i < size; i++){
        if(i > 0) t += matrix[pista[i-1].id - 1][pista[i].id - 1];
        if(t < pista[i].t_decolagem){ t += pista[i].t_decolagem - t; }
        else if(t > pista[i].t_decolagem){ multa += pista[i].multa * (t - pista[i].t_decolagem); }
        t += pista[i].duracao;
    }
    return multa;
}

void swap(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    int pista = dist_pista(gen);
    
    int tentativas = 0;
    while(tentativas < 100 && sol.pistas[pista].size() < 2) {
        pista = dist_pista(gen);
        tentativas++;
    }
    if (sol.pistas[pista].size() < 2) return;

    std::uniform_int_distribution<int> dist_pos(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_1 = dist_pos(gen);
    
    int pos_2 = pos_1;
    while(pos_2 == pos_1 && sol.pistas[pista].size() > 1) {
        pos_2 = dist_pos(gen);
    }

    std::swap(sol.pistas[pista][pos_1], sol.pistas[pista][pos_2]);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void swap_pistas(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
    int tentativas = 0;
    int pista_1, pista_2;
    do {
        pista_1 = dist_pista(gen);
        pista_2 = dist_pista(gen);
        tentativas++;
    } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].empty() || sol.pistas[pista_2].empty()));
    
    if (pista_1 == pista_2 || sol.pistas[pista_1].empty() || sol.pistas[pista_2].empty()) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    
    int pos_1 = dist_pos1(gen);
    int pos_2 = dist_pos2(gen);

    std::swap(sol.pistas[pista_1][pos_1], sol.pistas[pista_2][pos_2]);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void re_insertion(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
    int tentativas = 0;
    int pista;
    do {
        pista = dist_pista(gen);
        tentativas++;
    } while(tentativas < 100 && sol.pistas[pista].size() < 3);
    
    if (sol.pistas[pista].size() < 3) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_1 = dist_pos1(gen);
    
    voo voo_movido = sol.pistas[pista][pos_1];
    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1);

    int pos_2;
    do {
        pos_2 = dist_pos1(gen);
    } while(pos_2 == pos_1 && sol.pistas[pista].size() > 0);

    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void re_insertion_pistas(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
    int tentativas = 0;
    int pista_1, pista_2;
    do {
        pista_1 = dist_pista(gen);
        pista_2 = dist_pista(gen);
        tentativas++;
    } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].empty()));
    
    if (pista_1 == pista_2 || sol.pistas[pista_1].empty()) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
    std::uniform_int_distribution<int> dist_pos2(0, sol.pistas[pista_2].size());
    
    int pos_1 = dist_pos1(gen);
    int pos_2 = dist_pos2(gen);
    
    voo voo_movido = sol.pistas[pista_1][pos_1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void re_insertion_2(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
    int tentativas = 0;
    int pista;
    do {
        pista = dist_pista(gen);
        tentativas++;
    } while(tentativas < 100 && sol.pistas[pista].size() < 4);
    
    if (sol.pistas[pista].size() < 4) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 2);
    int pos_1 = dist_pos1(gen);

    voo voo_movido_1 = sol.pistas[pista][pos_1];
    voo voo_movido_2 = sol.pistas[pista][pos_1+1];
    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 2);

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_2;
    do {
        pos_2 = dist_pos2(gen);
    } while(pos_2 == pos_1 && sol.pistas[pista].size() > 1);

    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void re_insertion_2_pistas(solucao &sol, int** matrix, int num_pistas) {
    std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
    int tentativas = 0;
    int pista_1, pista_2;
    do {
        pista_1 = dist_pista(gen);
        pista_2 = dist_pista(gen);
        tentativas++;
    } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].size() < 2));
    
    if (pista_1 == pista_2 || sol.pistas[pista_1].size() < 2) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 2);
    int pos_1 = dist_pos1(gen);

    voo voo_movido_1 = sol.pistas[pista_1][pos_1];
    voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 2);

    std::uniform_int_distribution<int> dist_pos2(0, sol.pistas[pista_2].size());
    int pos_2 = dist_pos2(gen);

    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++) {
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }
    sol.multa = multa;
}

void sa(solucao &otimo, int** matrix, int num_pistas, int num_voos) {
    long double temperatura_inicial = calcula_temperatura_inicial(otimo, matrix, num_pistas);
    long double temperatura_final = temperatura_inicial / 500;
    long double temperatura = temperatura_inicial;
    long double coeficiente = 0.995;
    int max_iter = num_voos * 10;
    solucao s_atual = otimo;

    std::cout << "T0: " << temperatura_inicial << '\n';

    while(temperatura > temperatura_final){
        for(int l = 0; l < max_iter; l++){
            solucao s_ = generate_neighbor(s_atual, matrix, num_pistas);
            int variacao = s_.multa - s_atual.multa;

            if(variacao <= 0){
                s_atual = s_;

                if(s_atual.multa < otimo.multa){
                    otimo = s_atual;
                    std::cout << s_atual.multa << '\n';
                    l = 0;
                }
            } else{
                std::uniform_real_distribution<> dist(0, 1);
                double r = dist(gen);
                long double taxa = (((long double) variacao) /temperatura) * (-1);
                if(r < exp(taxa)){
                    s_atual = s_;
                }
            }
        }

        temperatura *= coeficiente;
        std::cout << "T: " << temperatura << '\n';
    }
}

long double calcula_temperatura_inicial(solucao &sol, int**matrix, int num_pistas){
    double sum_variacao = 0;

    for(int i = 0; i < 1000; i++){
        solucao s = generate_neighbor(sol, matrix, num_pistas);

        sum_variacao += (double) abs(s.multa - sol.multa) / sol.multa;
    }

    double media = (double) sum_variacao / 1000;

    return exp(media / 5);
}

solucao generate_neighbor(solucao sol, int**matrix, int num_pistas){
    std::uniform_int_distribution<int> dist(1, 6);

    int k = dist(gen);

    switch(k) {
        case 1:
            swap(sol, matrix, num_pistas);
            break;
        case 2:
            re_insertion(sol, matrix, num_pistas);
            break;
        case 3:
            swap_pistas(sol, matrix, num_pistas);
            break;
        case 4:
            re_insertion_pistas(sol, matrix, num_pistas);
            break;
        case 5:
            re_insertion_2(sol, matrix, num_pistas);
            break;
        case 6:
            re_insertion_2_pistas(sol, matrix, num_pistas);
            break;
    }

    return sol;
}