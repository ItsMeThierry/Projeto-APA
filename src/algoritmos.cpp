#include "include/algoritmos.h"

static std::random_device rd;
static std::mt19937 gen(rd());

Solucao algoritmo_guloso(const Dados &dados){
    Solucao sol(dados.num_pistas);

    std::vector<int> fila_voos(dados.num_voos);
    std::iota(fila_voos.begin(), fila_voos.end(), 0); //O(n)
    std::sort(fila_voos.begin(), fila_voos.end(), [&dados](int i1, int i2) { return dados.array_decolagem[i1] < dados.array_decolagem[i2]; });

    int t_pistas[dados.num_pistas] = {0};

    for(int i = 0; i < dados.num_voos; i++){
        Voo v(fila_voos[i] + 1);
        int menor_multa = INT_MAX;
        int pista_inserida = -1;
        int t_espera = 0;
        int t_vazio = 0;

        for(int pista = 0; pista < dados.num_pistas; pista++){
            int te = 0;
            int tv = 0;
            int multa = 0;

            if(!sol.pistas[pista].empty()) te = dados.matrix[sol.pistas[pista].back().id - 1][fila_voos[i]];
            if(t_pistas[pista] + te < dados.array_decolagem[fila_voos[i]]){ tv = dados.array_decolagem[fila_voos[i]] - (t_pistas[pista] + te); } 
            else if(t_pistas[pista] + te > dados.array_decolagem[fila_voos[i]]){ multa = dados.array_penalidade[fila_voos[i]] * (t_pistas[pista] + te - dados.array_decolagem[fila_voos[i]]); }

            if(multa < menor_multa){
                menor_multa = multa;
                pista_inserida = pista;
                t_espera = te;
                t_vazio = tv;
            }
        }

        v.t_decolagem = t_pistas[pista_inserida] + t_espera + t_vazio;
        t_pistas[pista_inserida] += t_espera + t_vazio + dados.array_duracao[fila_voos[i]];
        sol.multa_pistas[pista_inserida] += menor_multa;
        v.multa_acumulada = sol.multa_pistas[pista_inserida];
        sol.multa += menor_multa;
        sol.pistas[pista_inserida].push_back(v);
    }

    return sol;
}

// int calcula_multa_pista(const std::vector<Voo> &pista){
//     int multa = 0;

//     for(Voo v : pista){ multa += v.multa_acumulada; } //O(n)
    
//     return multa;
// }

// void calcula_tempo_e_multa(const std::vector<Voo> &pista, int &multa, int &t, int pos_anterior, int pos_atual, const Dados& dados){
//     if(pos_anterior > 0) t += dados.matrix[pista[pos_anterior].id - 1][pista[pos_atual].id - 1];
//     if(t < dados.array_decolagem[pista[pos_atual].id - 1]){ t += dados.array_decolagem[pista[pos_atual].id - 1] - t; }
//     else if(t > dados.array_decolagem[pista[pos_atual].id - 1]){ multa += dados.array_penalidade[pista[pos_atual].id - 1] * (t - dados.array_decolagem[pista[pos_2].id - 1]); }
//     t += dados.array_duracao[pista[pos_atual].id - 1];
// }

int calcula_multa_pista(const std::vector<Voo> &pista, int pos_1, int pos_2, const Dados& dados){
    int multa = (pos_1 == 0) ? 0 : pista[pos_1 - 1].multa_acumulada;
    int t = (pos_1 == 0) ? 0 : pista[pos_1 - 1].t_decolagem + dados.array_duracao[pista[pos_1 - 1].id - 1];

    for(int i = pos_1; i < pos_2; i++){
        if(i > 0) t += dados.matrix[pista[i-1].id - 1][pista[i].id - 1];
        if(t < dados.array_decolagem[pista[i].id - 1]){ t += dados.array_decolagem[pista[i].id - 1] - t; }
        else if(t > dados.array_decolagem[pista[i].id - 1]){ multa += dados.array_penalidade[pista[i].id - 1] * (t - dados.array_decolagem[pista[i].id - 1]); }
        t += dados.array_duracao[pista[i].id - 1];
    }

    return multa;
}

void calcula_multa_pista(const std::vector<Voo> &pista, int &multa, int &t, int pos_1, int pos_2, const Dados& dados){
    if(pos_1 > (int) pista.size() || pos_2 > (int) pista.size()){
        std::cout << "[ERRO] A pista possui tamanho " << pista.size() << ", pos_1 = " << pos_1 << " pos_2 = " << pos_2 << std::endl;
    }

    for(int i = pos_1; i < pos_2; i++){
        if(i > 0) t += dados.matrix[pista[i-1].id - 1][pista[i].id - 1];
        if(t < dados.array_decolagem[pista[i].id - 1]){ t += dados.array_decolagem[pista[i].id - 1] - t; }
        else if(t > dados.array_decolagem[pista[i].id - 1]){ multa += dados.array_penalidade[pista[i].id - 1] * (t - dados.array_decolagem[pista[i].id - 1]); }
        t += dados.array_duracao[pista[i].id - 1];
    }
}

void atualiza_voos(std::vector<Voo> &pista, int pos_alterada, const Dados& dados){
    int multa = (pos_alterada == 0) ? 0 : pista[pos_alterada - 1].multa_acumulada;
    int t = (pos_alterada == 0) ? 0 : pista[pos_alterada - 1].t_decolagem + dados.array_duracao[pista[pos_alterada - 1].id - 1];
    int size = pista.size();

    for(int i = pos_alterada; i < size; i++){
        if(i > 0) t += dados.matrix[pista[i-1].id - 1][pista[i].id - 1];
        if(t < dados.array_decolagem[pista[i].id - 1]){ t += dados.array_decolagem[pista[i].id - 1] - t; }
        else if(t > dados.array_decolagem[pista[i].id - 1]){ multa += dados.array_penalidade[pista[i].id - 1] * (t - dados.array_decolagem[pista[i].id - 1]); }
        pista[i].t_decolagem = t;
        t += dados.array_duracao[pista[i].id - 1];
        pista[i].multa_acumulada = multa;
    }
}

// void vnd(Solucao &otimo, const Dados &dados){
//     int k = 1;
//     int menor_multa = otimo.multa;

//     while(k <= 6){
//         switch(k){
//             case 1:
//                 swap(otimo, dados);
//                 break;
//             case 2:
//                 re_insertion(otimo, dados);
//                 break;
//             case 3:
//                 re_insertion_2(otimo, dados);
//                 break;
//             case 4:
//                 re_insertion_3(otimo, dados);
//                 break;
//             case 5:
//                 swap_pistas(otimo, dados);
//                 break;
//             case 6:
//                 re_insertion_pistas(otimo, dados);
//                 break;
//             case 7:
//                 re_insertion_2_pistas(otimo, dados);
//                 break;
//             case 8:
//                 re_insertion_3_pistas(otimo, dados);
//                 break;
//         }

//         if(otimo.multa < menor_multa){
//             menor_multa = otimo.multa;
//             // std::cout << menor_multa << std::endl;
//             k = 1;
//         }else{
//             k++;
//         }
//     }
// }

void swap(Solucao &sol, const Dados &dados) {
    int menor_multa = INT_MAX;
    int pista = -1;
    int pos_origem = -1;
    int pos_destino = -1;

    for(int i = 0; i < dados.num_pistas; i++){
        int size = sol.pistas[i].size();

        if(size == 1){
            continue;
        }

        for (int origem = 0; origem < size - 1; origem++) {
            for (int destino = origem + 1; destino < size; destino++){
        
                std::swap(sol.pistas[i][origem], sol.pistas[i][destino]);

                int multa = calcula_multa_pista(sol.pistas[i], origem, size, dados); //O(n^2) no total

                std::swap(sol.pistas[i][destino], sol.pistas[i][origem]);
                
                if (multa < menor_multa && multa < sol.multa_pistas[i]) {
                    menor_multa = multa;
                    pista = i;
                    pos_origem = origem;
                    pos_destino = destino;
                }
            }
        }
    }

    if(pista != -1){
        std::swap(sol.pistas[pista][pos_origem], sol.pistas[pista][pos_destino]);

        atualiza_voos(sol.pistas[pista], pos_origem, dados); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[pista];
        sol.multa += menor_multa;

        sol.multa_pistas[pista] = menor_multa;
    }
}

void swap_pistas(Solucao &sol, const Dados& dados){
    int multa_origem = INT_MAX - 1;
    int multa_destino = 1;
    int pista_origem = -1;
    int pista_destino = -1;
    int pos_origem = -1;
    int pos_destino = -1;

    for(int origem = 0; origem < dados.num_pistas - 1; origem++){
        int size_origem = sol.pistas[origem].size();

        for(int pos_1 = 0; pos_1 < size_origem; pos_1++){
            for(int destino = origem + 1; destino < dados.num_pistas; destino++){
                int size_destino = sol.pistas[destino].size();

                for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                    
                    std::swap(sol.pistas[origem][pos_1], sol.pistas[destino][pos_2]);

                    int nova_multa_origem = calcula_multa_pista(sol.pistas[origem], pos_1, size_origem, dados);
                    int nova_multa_destino = calcula_multa_pista(sol.pistas[destino], pos_2, size_destino, dados);

                    std::swap(sol.pistas[destino][pos_2], sol.pistas[origem][pos_1]);

                    if(nova_multa_origem + nova_multa_destino < multa_origem + multa_destino && nova_multa_origem + nova_multa_destino < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                        multa_origem = nova_multa_origem;
                        multa_destino = nova_multa_destino;
                        pista_origem = origem;
                        pista_destino = destino;
                        pos_origem = pos_1;
                        pos_destino = pos_2;
                    }
                }
            }
        }
    }

    if(pista_origem != -1 && pista_destino != -1){
        std::swap(sol.pistas[pista_origem][pos_origem], sol.pistas[pista_destino][pos_destino]);

        atualiza_voos(sol.pistas[pista_origem], pos_origem, dados); //O(n) no pior caso
        atualiza_voos(sol.pistas[pista_destino], pos_destino, dados); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[pista_origem] + sol.multa_pistas[pista_destino];
        sol.multa += multa_origem + multa_destino;

        sol.multa_pistas[pista_origem] = multa_origem;
        sol.multa_pistas[pista_destino] = multa_destino;
    }
}

void re_insertion(Solucao &sol, const Dados &dados) {
    int menor_multa = INT_MAX;
    int pista = -1;
    int pos_origem = -1;
    int pos_destino = -1;

    for(int i = 0; i < dados.num_pistas; i++) {
        int size = sol.pistas[i].size();

        if (size < 2){
            continue;
        }

        for(int origem = 0; origem < size; origem++){
            for(int destino = 0; destino < size; destino++){
                if(origem == destino) continue;
                if(abs(destino - origem) == 1) continue;
                int multa = 0;

                if(origem < destino){
                    multa = (origem == 0) ? 0 : sol.pistas[i][origem - 1].multa_acumulada;
                    int t = (origem == 0) ? 0 : sol.pistas[i][origem - 1].t_decolagem + dados.array_duracao[sol.pistas[i][origem - 1].id - 1];
                    
                    if(origem > 0) t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 1].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 1].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem + 1].id - 1];

                    calcula_multa_pista(sol.pistas[i], multa, t, origem + 2, destino + 1, dados);

                    t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem].id - 1];

                    if(destino != size - 1){
                        t += dados.matrix[sol.pistas[i][origem].id - 1][sol.pistas[i][destino + 1].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 1].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][destino + 1].id - 1];

                        if(destino + 2 < size) calcula_multa_pista(sol.pistas[i], multa, t, destino + 2, size, dados);
                    }

                } else{
                    multa = (destino == 0) ? 0 : sol.pistas[i][destino - 1].multa_acumulada;
                    int t = (destino == 0) ? 0 : sol.pistas[i][destino - 1].t_decolagem + dados.array_duracao[sol.pistas[i][destino - 1].id - 1];

                    if(destino > 0) t += dados.matrix[sol.pistas[i][destino-1].id - 1][sol.pistas[i][origem].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem].id - 1];

                    t += dados.matrix[sol.pistas[i][origem].id - 1][sol.pistas[i][destino].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][destino].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][destino].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][destino].id - 1];

                    calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, origem, dados);
                    calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, size, dados);
                }

                if(multa < menor_multa && multa < sol.multa_pistas[i]){
                    menor_multa = multa;
                    pista = i;
                    pos_origem = origem;
                    pos_destino = destino;
                }
            }
        }
    }

    if(pista != -1){
        Voo voo_realocado = sol.pistas[pista][pos_origem];
        sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_origem);
        sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_destino, voo_realocado);

        if(pos_origem < pos_destino){
            atualiza_voos(sol.pistas[pista], pos_origem, dados); //O(n) no pior caso
        } else{
            atualiza_voos(sol.pistas[pista], pos_destino, dados); //O(n) no pior caso
        }
        
        sol.multa -= sol.multa_pistas[pista];
        sol.multa += menor_multa;

        sol.multa_pistas[pista] = menor_multa;
    }
}

void re_insertion_pistas(Solucao &sol, const Dados &dados) {
    int melhor_multa_origem = INT_MAX - 1;
    int melhor_multa_destino = 1;
    int melhor_pista_origem = -1, melhor_pos_mover = -1;
    int melhor_pista_destino = -1, melhor_nova_pos = -1;
    
    for(int origem = 0; origem < dados.num_pistas; origem++){
        int size_origem = sol.pistas[origem].size();

        for(int pos_1 = 0; pos_1 < size_origem; pos_1++){

            for(int destino = 0; destino < dados.num_pistas; destino++){
                if(origem == destino) continue;
                int size_destino = sol.pistas[destino].size();

                for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                    // Calcula remoção em pista origem
                    int multa_origem = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].multa_acumulada;
                    int t_1 = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].t_decolagem + dados.array_duracao[sol.pistas[origem][pos_1 - 1].id - 1];

                    if(pos_1 != size_origem - 1){
                        if(pos_1 > 0) t_1 += dados.matrix[sol.pistas[origem][pos_1 - 1].id - 1][sol.pistas[origem][pos_1 + 1].id - 1];
                        if(t_1 < dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]){ t_1 += dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1] - t_1; }
                        else if(t_1 > dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]){ multa_origem += dados.array_penalidade[sol.pistas[origem][pos_1 + 1].id - 1] * (t_1 - dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]); }
                        t_1 += dados.array_duracao[sol.pistas[origem][pos_1 + 1].id - 1];

                        calcula_multa_pista(sol.pistas[origem], multa_origem, t_1, pos_1 + 2, size_origem, dados);
                    }

                    // Calcula inserção em pista destino
                    int multa_destino = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].multa_acumulada;
                    int t_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].t_decolagem + dados.array_duracao[sol.pistas[destino][pos_2 - 1].id - 1];

                    // std::cout << multa_destino << ' ';

                    if(pos_2 > 0) t_2 += dados.matrix[sol.pistas[destino][pos_2 - 1].id - 1][sol.pistas[origem][pos_1].id - 1];
                    if(t_2 < dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[origem][pos_1].id - 1] - t_2; }
                    else if(t_2 > dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[origem][pos_1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]); }
                    t_2 += dados.array_duracao[sol.pistas[origem][pos_1].id - 1];

                    // std::cout << multa_destino << ' ';

                    t_2 += dados.matrix[sol.pistas[origem][pos_1].id - 1][sol.pistas[destino][pos_2].id - 1];
                    if(t_2 < dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[destino][pos_2].id - 1] - t_2; }
                    else if(t_2 > dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[destino][pos_2].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]); }
                    t_2 += dados.array_duracao[sol.pistas[destino][pos_2].id - 1];

                    // std::cout << multa_destino << ' ';

                    calcula_multa_pista(sol.pistas[destino], multa_destino, t_2, pos_2 + 1, size_destino, dados);
                        
                    // std::cout << multa_destino << ' ';

                    if(multa_origem + multa_destino < melhor_multa_origem + melhor_multa_destino && multa_origem + multa_destino < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                        melhor_multa_origem = multa_origem;
                        melhor_multa_destino = multa_destino;
                        melhor_pista_origem = origem;
                        melhor_pista_destino = destino;
                        melhor_pos_mover = pos_1;
                        melhor_nova_pos = pos_2;
                    }
                }
            }

        }

    }

    if(melhor_pista_origem != -1 && melhor_pista_destino != -1){
        Voo voo_realocado = sol.pistas[melhor_pista_origem][melhor_pos_mover];
        sol.pistas[melhor_pista_origem].erase(sol.pistas[melhor_pista_origem].begin() + melhor_pos_mover);
        sol.pistas[melhor_pista_destino].insert(sol.pistas[melhor_pista_destino].begin() + melhor_nova_pos, voo_realocado);

        atualiza_voos(sol.pistas[melhor_pista_origem], melhor_pos_mover, dados); //O(n) no pior caso
        atualiza_voos(sol.pistas[melhor_pista_destino], melhor_nova_pos, dados); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[melhor_pista_origem] + sol.multa_pistas[melhor_pista_destino];
        sol.multa += melhor_multa_origem + melhor_multa_destino;

        sol.multa_pistas[melhor_pista_origem] = melhor_multa_origem;
        sol.multa_pistas[melhor_pista_destino] = melhor_multa_destino;
    }

}

void re_insertion_2(Solucao &sol, const Dados &dados){
    int menor_multa = INT_MAX;
    int pista = -1;
    int pos_origem = -1;
    int pos_destino = -1;

    for(int i = 0; i < dados.num_pistas; i++) {
        int size = sol.pistas[i].size();

        if (size < 4){
            continue;
        }

        for(int origem = 0; origem < size - 1; origem++){
            for(int destino = 0; destino < size - 2; destino++){
                if(origem == destino) continue;
                if(abs(destino - origem) == 1) continue;
                int multa = 0;

                if(origem < destino){
                    multa = (origem == 0) ? 0 : sol.pistas[i][origem - 1].multa_acumulada;
                    int t = (origem == 0) ? 0 : sol.pistas[i][origem - 1].t_decolagem + dados.array_duracao[sol.pistas[i][origem - 1].id - 1];
                    
                    if(origem > 0) t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 2].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 2].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 2].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem + 2].id - 1];
                    
                    calcula_multa_pista(sol.pistas[i], multa, t, origem + 3, destino + 2, dados);
                    
                    t += dados.matrix[sol.pistas[i][destino + 1].id - 1][sol.pistas[i][origem].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem].id - 1];

                    calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 2, dados);
                    
                    if(destino != size - 2){
                        t += dados.matrix[sol.pistas[i][origem + 1].id - 1][sol.pistas[i][destino + 2].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][destino + 2].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 2].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][destino + 2].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 2].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 2].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][destino + 2].id - 1];
                        
                        if(destino + 3 < size) calcula_multa_pista(sol.pistas[i], multa, t, destino + 3, size, dados);
                        
                    }

                } else{
                    multa = (destino == 0) ? 0 : sol.pistas[i][destino - 1].multa_acumulada;
                    int t = (destino == 0) ? 0 : sol.pistas[i][destino - 1].t_decolagem + dados.array_duracao[sol.pistas[i][destino - 1].id - 1];

                    if(destino > 0) t += dados.matrix[sol.pistas[i][destino-1].id - 1][sol.pistas[i][origem].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][origem].id - 1];
                    
                    calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 2, dados);

                    t += dados.matrix[sol.pistas[i][origem + 1].id - 1][sol.pistas[i][destino].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][destino].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][destino].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][destino].id - 1];
                    
                    calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, origem, dados);
                    if(origem != size - 2){
                        t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 2].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 2].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 2].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem + 2].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 3, size, dados);
                        
                    }
                }

                if(multa < menor_multa && multa < sol.multa_pistas[i]){
                    menor_multa = multa;
                    pista = i;
                    pos_origem = origem;
                    pos_destino = destino;
                }
            }
        }
    }

    if(pista != -1){
        Voo voo_realocado_1 = sol.pistas[pista][pos_origem];
        Voo voo_realocado_2 = sol.pistas[pista][pos_origem+1];

        sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_origem, sol.pistas[pista].begin() + pos_origem + 2);
        sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_destino, voo_realocado_1);
        sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_destino + 1, voo_realocado_2);

        if(pos_origem < pos_destino){
            atualiza_voos(sol.pistas[pista], pos_origem, dados); //O(n) no pior caso
        } else{
            atualiza_voos(sol.pistas[pista], pos_destino, dados); //O(n) no pior caso
        }
        
        sol.multa -= sol.multa_pistas[pista];
        sol.multa += menor_multa;

        sol.multa_pistas[pista] = menor_multa;
    }
}

// void re_insertion_2_pistas(Solucao &sol, int**matrix, int num_pistas){
//     int menor_multa = INT_MAX;
//     std::vector<voo> pista_origem_alterada;
//     std::vector<voo> pista_destino_alterada;
//     int pista_destino = -1;
//     int pista_origem = -1;

//     for(int i = 0; i < num_pistas; i++){
//         int size = sol.pistas[i].size();
        
//         if(size < 2) continue;

//         for(int pos_selected = 0; pos_selected < size - 1; pos_selected++){

//             std::vector<voo> pista_sobra = sol.pistas[i];

//             pista_sobra.erase(pista_sobra.begin() + pos_selected, pista_sobra.begin() + pos_selected + 2);

//             int multa_origem = calcula_multa_pista(pista_sobra, matrix);

//             for(int destino = 0; destino < num_pistas; destino++){
//                 if(destino == i) continue;
//                 int destino_size = sol.pistas[destino].size();

//                 for(int pos_insert = 0; pos_insert <= destino_size; pos_insert++){
//                     std::vector<voo> modificada = sol.pistas[destino];
//                     modificada.insert(modificada.begin() + pos_insert, sol.pistas[i][pos_selected]);
//                     modificada.insert(modificada.begin() + pos_insert + 1, sol.pistas[i][pos_selected+1]);

//                     int multa_destino = calcula_multa_pista(modificada, matrix);

//                     if(multa_origem+multa_destino < menor_multa){
//                         menor_multa = multa_origem + multa_destino;
//                         pista_origem = i;
//                         pista_destino = destino;
//                         pista_origem_alterada = pista_sobra;
//                         pista_destino_alterada = modificada;
//                     }
//                 }
//             }

//         }
//     }

//     if(pista_destino != -1){
//         int multa_origem_antes = calcula_multa_pista(sol.pistas[pista_origem], matrix);
//         int multa_destino_antes = calcula_multa_pista(sol.pistas[pista_destino], matrix);

//         if(menor_multa < multa_origem_antes + multa_destino_antes){
//             sol.pistas[pista_origem] = pista_origem_alterada;
//             sol.pistas[pista_destino] = pista_destino_alterada;
//             sol.multa -= multa_origem_antes + multa_destino_antes;
//             sol.multa += menor_multa;
//         }
//     }
// }

// void re_insertion_3(Solucao &sol, int**matrix, int num_pistas){
//     int menor_multa = INT_MAX;
//     std::vector<voo> pista_alterada;
//     int pista_id = -1;

//     for(int i = 0; i < num_pistas; i++){
//         int size = sol.pistas[i].size();

//         if(size == 4) continue;       

//         for(int pos_selected = 0; pos_selected < size - 2; pos_selected++){
//             std::vector<voo> sobra = sol.pistas[i];

//             sobra.erase(sobra.begin() + pos_selected, sobra.begin() + pos_selected + 3);

//             int new_size = sobra.size();

//             for(int pos_insert = 0; pos_insert <= new_size; pos_insert++){
//                 if(pos_insert == pos_selected) continue;
//                 std::vector<voo> modificada = sobra;
//                 modificada.insert(modificada.begin() + pos_insert, sol.pistas[i][pos_selected]);
//                 modificada.insert(modificada.begin() + pos_insert + 1, sol.pistas[i][pos_selected+1]);
//                 modificada.insert(modificada.begin() + pos_insert + 2, sol.pistas[i][pos_selected+2]);

//                 int multa = calcula_multa_pista(modificada, matrix);

//                 if(multa < menor_multa){
//                     menor_multa = multa;
//                     pista_id = i;
//                     pista_alterada = modificada;
//                 }
//             }

//         }
//     }

//     if(pista_id != -1){
//         int multa_antes = calcula_multa_pista(sol.pistas[pista_id], matrix);

//         if(menor_multa < multa_antes){
//             sol.pistas[pista_id] = pista_alterada;
//             sol.multa -= multa_antes;
//             sol.multa += menor_multa;
//         }
//     }
// }

// void re_insertion_3_pistas(Solucao &sol, int**matrix, int num_pistas){
//     int menor_multa = INT_MAX;
//     std::vector<voo> pista_origem_alterada;
//     std::vector<voo> pista_destino_alterada;
//     int pista_destino = -1;
//     int pista_origem = -1;

//     for(int i = 0; i < num_pistas; i++){
//         int size = sol.pistas[i].size();

//         if(size < 3) continue;       

//         for(int pos_selected = 0; pos_selected < size - 2; pos_selected++){

//             std::vector<voo> pista_sobra = sol.pistas[i];

//             pista_sobra.erase(pista_sobra.begin() + pos_selected, pista_sobra.begin() + pos_selected + 3);

//             int multa_origem = calcula_multa_pista(pista_sobra, matrix);

//             for(int destino = 0; destino < num_pistas; destino++){
//                 if(destino == i) continue;
//                 int destino_size = sol.pistas[destino].size();

//                 for(int pos_insert = 0; pos_insert <= destino_size; pos_insert++){
//                     std::vector<voo> modificada = sol.pistas[destino];
//                     modificada.insert(modificada.begin() + pos_insert, sol.pistas[i][pos_selected]);
//                     modificada.insert(modificada.begin() + pos_insert + 1, sol.pistas[i][pos_selected+1]);
//                     modificada.insert(modificada.begin() + pos_insert + 2, sol.pistas[i][pos_selected+2]);

//                     int multa_destino = calcula_multa_pista(modificada, matrix);

//                     if(multa_origem+multa_destino < menor_multa){
//                         menor_multa = multa_origem + multa_destino;
//                         pista_origem = i;
//                         pista_destino = destino;
//                         pista_origem_alterada = pista_sobra;
//                         pista_destino_alterada = modificada;
//                     }
//                 }
//             }

//         }
//     }

//     if(pista_destino != -1){
//         int multa_origem_antes = calcula_multa_pista(sol.pistas[pista_origem], matrix);
//         int multa_destino_antes = calcula_multa_pista(sol.pistas[pista_destino], matrix);

//         if(menor_multa < multa_origem_antes + multa_destino_antes){
//             sol.pistas[pista_origem] = pista_origem_alterada;
//             sol.pistas[pista_destino] = pista_destino_alterada;
//             sol.multa -= multa_origem_antes + multa_destino_antes;
//             sol.multa += menor_multa;
//         }
//     }
// }


// void rand_swap(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
//     int pista = dist_pista(gen);
    
//     int tentativas = 0;
//     while(tentativas < 100 && sol.pistas[pista].size() < 2) {
//         pista = dist_pista(gen);
//         tentativas++;
//     }
//     if (sol.pistas[pista].size() < 2) return;

//     std::uniform_int_distribution<int> dist_pos(0, ((int) sol.pistas[pista].size()) - 1);
//     int pos_1 = dist_pos(gen);
    
//     int pos_2 = pos_1;
//     while(pos_2 == pos_1 && sol.pistas[pista].size() > 1) {
//         pos_2 = dist_pos(gen);
//     }

//     std::swap(sol.pistas[pista][pos_1], sol.pistas[pista][pos_2]);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_swap_pistas(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista_1, pista_2;
//     do {
//         pista_1 = dist_pista(gen);
//         pista_2 = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].empty() || sol.pistas[pista_2].empty()));
    
//     if (pista_1 == pista_2 || sol.pistas[pista_1].empty() || sol.pistas[pista_2].empty()) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
//     std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    
//     int pos_1 = dist_pos1(gen);
//     int pos_2 = dist_pos2(gen);

//     std::swap(sol.pistas[pista_1][pos_1], sol.pistas[pista_2][pos_2]);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista;
//     do {
//         pista = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && sol.pistas[pista].size() < 3);
    
//     if (sol.pistas[pista].size() < 3) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 1);
//     int pos_1 = dist_pos1(gen);
    
//     voo voo_movido = sol.pistas[pista][pos_1];
//     sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1);

//     int pos_2;
//     do {
//         pos_2 = dist_pos1(gen);
//     } while(pos_2 == pos_1 && sol.pistas[pista].size() > 0);

//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion_pistas(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista_1, pista_2;
//     do {
//         pista_1 = dist_pista(gen);
//         pista_2 = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].empty()));
    
//     if (pista_1 == pista_2 || sol.pistas[pista_1].empty()) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
//     std::uniform_int_distribution<int> dist_pos2(0, sol.pistas[pista_2].size());
    
//     int pos_1 = dist_pos1(gen);
//     int pos_2 = dist_pos2(gen);
    
//     voo voo_movido = sol.pistas[pista_1][pos_1];
//     sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1);
//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion_2(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista;
//     do {
//         pista = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && sol.pistas[pista].size() < 4);
    
//     if (sol.pistas[pista].size() < 4) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 2);
//     int pos_1 = dist_pos1(gen);

//     voo voo_movido_1 = sol.pistas[pista][pos_1];
//     voo voo_movido_2 = sol.pistas[pista][pos_1+1];
//     sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 2);

//     std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista].size()) - 1);
//     int pos_2;
//     do {
//         pos_2 = dist_pos2(gen);
//     } while(pos_2 == pos_1 && sol.pistas[pista].size() > 1);

//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion_2_pistas(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista_1, pista_2;
//     do {
//         pista_1 = dist_pista(gen);
//         pista_2 = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].size() < 2));
    
//     if (pista_1 == pista_2 || sol.pistas[pista_1].size() < 2) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 2);
//     int pos_1 = dist_pos1(gen);

//     voo voo_movido_1 = sol.pistas[pista_1][pos_1];
//     voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
//     sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 2);

//     std::uniform_int_distribution<int> dist_pos2(0, sol.pistas[pista_2].size());
//     int pos_2 = dist_pos2(gen);

//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion_3(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista;
//     do {
//         pista = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && sol.pistas[pista].size() < 5);
    
//     if (sol.pistas[pista].size() < 5) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 3);
//     int pos_1 = dist_pos1(gen);

//     voo voo_movido_1 = sol.pistas[pista][pos_1];
//     voo voo_movido_2 = sol.pistas[pista][pos_1+1];
//     voo voo_movido_3 = sol.pistas[pista][pos_1+2];
//     sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 3);

//     std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista].size()) - 1);
//     int pos_2;
//     do {
//         pos_2 = dist_pos2(gen);
//     } while(pos_2 == pos_1 && sol.pistas[pista].size() > 1);

//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);
//     sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 2, voo_movido_3);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void rand_re_insertion_3_pistas(Solucao &sol, int** matrix, int num_pistas) {
//     std::uniform_int_distribution<int> dist_pista(0, num_pistas - 1);
    
//     int tentativas = 0;
//     int pista_1, pista_2;
//     do {
//         pista_1 = dist_pista(gen);
//         pista_2 = dist_pista(gen);
//         tentativas++;
//     } while(tentativas < 100 && (pista_1 == pista_2 || sol.pistas[pista_1].size() < 3));
    
//     if (pista_1 == pista_2 || sol.pistas[pista_1].size() < 3) return;

//     std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 3);
//     int pos_1 = dist_pos1(gen);

//     voo voo_movido_1 = sol.pistas[pista_1][pos_1];
//     voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
//     voo voo_movido_3 = sol.pistas[pista_1][pos_1+2];
//     sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 3);

//     std::uniform_int_distribution<int> dist_pos2(0, sol.pistas[pista_2].size());
//     int pos_2 = dist_pos2(gen);

//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);
//     sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 2, voo_movido_3);

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++) {
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }
//     sol.multa = multa;
// }

// void sa(Solucao &otimo, int** matrix, int num_pistas, int num_voos) {
//     long double temperatura_inicial = calcula_temperatura_inicial(otimo, matrix, num_pistas);
//     long double temperatura_final = temperatura_inicial / num_voos;
//     long double temperatura = temperatura_inicial;
//     long double coeficiente = 0.995;
//     int max_iter = num_voos * 50;
//     Solucao s_atual = otimo;
    
//     std::cout << "T0: " << temperatura_inicial << '\n';

//     while(temperatura > temperatura_final){
//         for(int l = 0; l < max_iter; l++){
//             Solucao s_ = generate_neighbor(s_atual, matrix, num_pistas);
//             int variacao = s_.multa - s_atual.multa;

//             if(variacao <= 0){
//                 s_atual = s_;

//                 if(s_atual.multa < otimo.multa){
//                     otimo = s_atual;
//                     std::cout << s_atual.multa << '\n';
//                     l = 0;
//                 }
//             } else{
//                 std::uniform_real_distribution<> dist(0, 1);
//                 double r = dist(gen);
//                 long double taxa = (((long double) variacao) / temperatura) * (-1);
//                 // std::cout << "taxa " << taxa << "\n";
//                 if(r < exp(taxa)){
//                     s_atual = s_;
//                     // std::cout << s_atual.multa << '\n';
//                 }
//             }
//         }

//         temperatura *= coeficiente;
//         std::cout << "T: " << temperatura << '\n';
//     }
// }

// long double calcula_temperatura_inicial(Solucao &sol, int**matrix, int num_pistas){
//     double sum_variacao = 0;

//     for(int i = 0; i < 1000; i++){
//         Solucao s = generate_neighbor(sol, matrix, num_pistas);

//         sum_variacao += (double) abs(s.multa - sol.multa) / sol.multa;
//     }

//     double media = (double) sum_variacao / 1000;

//     return exp(media / 5);
// }

// Solucao generate_neighbor(Solucao sol, int**matrix, int num_pistas){
//     std::uniform_int_distribution<int> dist(1, 8);

//     int k = dist(gen);

//     switch(k) {
//         case 1:
//             rand_swap(sol, matrix, num_pistas);
//             break;
//         case 2:
//             rand_re_insertion(sol, matrix, num_pistas);
//             break;
//         case 3:
//             rand_swap_pistas(sol, matrix, num_pistas);
//             break;
//         case 4:
//             rand_re_insertion_pistas(sol, matrix, num_pistas);
//             break;
//         case 5:
//             rand_re_insertion_2(sol, matrix, num_pistas);
//             break;
//         case 6:
//             rand_re_insertion_2_pistas(sol, matrix, num_pistas);
//             break;
//         case 7:
//             rand_re_insertion_3(sol, matrix, num_pistas);
//             break;
//         case 8:
//             rand_re_insertion_3_pistas(sol, matrix, num_pistas);
//             break;
//     }

//     return sol;
// }

// void ils(Solucao &s, int** matrix, int num_pistas){
//     int tentativa = 1;
//     Solucao melhor_s = s;

//     while(tentativa < num_pistas){
//         Solucao s_ = pertubacao(s, matrix, num_pistas);
//         vnd(s_, matrix, num_pistas);

//         if(s_.multa < melhor_s.multa){
//             std::cout << s_.multa << std::endl;
//             melhor_s = s_;
//             s = melhor_s;
//             tentativa = 1;
//         } else{
//             tentativa++;
//             s = s_;
//         }
//     }

//     s = melhor_s;
// }

// // Solucao pertubacao(Solucao sol, int**matrix, int num_pistas){
// //     for(int i = 0; i < num_pistas; i++){
// //         rand_swap_pistas(sol, matrix, num_pistas);
// //         rand_re_insertion_2_pistas(sol, matrix, num_pistas);
// //         // rand_re_insertion_3_pistas(sol, matrix, num_pistas);
// //     }

// //     int multa = 0;
// //     for(int i = 0; i < num_pistas; i++){
// //         multa += calcula_multa_pista(sol.pistas[i], matrix);
// //     }

// //     sol.multa = multa;
    
// //     return sol;
// // }

// Solucao pertubacao(Solucao sol, int**matrix, int num_pistas){
//     if(num_pistas == 1){
//         return sol;
//     }

//     std::vector<voo> voos_deslocados[num_pistas];

//     for(int i = 0; i < num_pistas; i++){
//         for(int j = 0; j < 3; j++){
//             if(sol.pistas[i].size() > 0){
//                 voos_deslocados[i].push_back(sol.pistas[i].front());
//                 sol.pistas[i].erase(sol.pistas[i].begin());
//             }
//         }
//     }

//     for(int i = 0; i < num_pistas; i++){
//         int size = voos_deslocados[(i+1) % num_pistas].size();

//         for(int j = size - 1; j >= 0; j--){
//             sol.pistas[i].insert(sol.pistas[i].begin(), voos_deslocados[(i+1) % num_pistas][j]);
//         }
//     }

//     int multa = 0;
//     for(int i = 0; i < num_pistas; i++){
//         multa += calcula_multa_pista(sol.pistas[i], matrix);
//     }

//     sol.multa = multa;
    
//     return sol;
// }