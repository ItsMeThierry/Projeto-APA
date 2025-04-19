#include "include/algoritmos.h"

void algoritmo_guloso(solucao &sol, voo* voos, int** matrix, int num_voos, int num_pistas){

    // Insertion sort, organizando o vetor de voos por ordem crescente de decolagem
    for(int i = 1; i < num_voos; i++){
        for (int j = i; j > 0; j--){
            if(voos[j-1].t_decolagem > voos[j].t_decolagem){
                voo aux = voos[j-1];
                voos[j-1] = voos[j];
                voos[j] = aux;
            }
        }
    }

    int t_pistas[num_pistas]; // Representa o tempo da pista
    int multa = 0; // Valor da multa

    sol.pistas = new std::vector<voo>[num_pistas]; // Criando um espaço na memória pras pistas da solução
    sol.voos_multa = new int[num_voos]; // Criando um espaço na memória pras multas de cada voo

    for (int i = 0; i < num_pistas; i++){
        t_pistas[i] = 0;
    }

    for (int i = 0; i < num_voos; i++){
        sol.voos_multa[i] = 0;
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


        if(menor_multa != 0){
            sol.voos_multa[voos[i].id-1] = menor_multa;
        }
    }

    sol.multa = multa;
}

solucao two_opt(solucao sol, int**matrix, int num_pistas){
    for(int i = 0; i < num_pistas; i++){
        int size = sol.pistas[i].size();

        if(size < 4){
            continue;
        }

        int menor_multa = 0;

        for(voo v : sol.pistas[i]){
            menor_multa += sol.voos_multa[v.id-1];
        }

        int multa_antes = menor_multa;
        int pos_inicial = -1;

        for(int start = 0; start + 3 < size; start++){
            int multa = 0;
            int t = 0;

            for(int j = 0; j < start; j++){
                if(j > 0) t += matrix[sol.pistas[i][j-1].id - 1][sol.pistas[i][j].id - 1];
                if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                t += sol.pistas[i][j].duracao;
            }

            for(int j = start + 3; j >= 0; j--){
                if(t > 0) t += (j - 3 == start) ? matrix[sol.pistas[i][start - 1].id - 1][sol.pistas[i][j].id - 1] : matrix[sol.pistas[i][j+1].id - 1][sol.pistas[i][j].id - 1];
                if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                t += sol.pistas[i][j].duracao;
            }

            for(int j = start + 4; j < size; j++){
                t += (j == start + 4) ? matrix[sol.pistas[i][start].id - 1][sol.pistas[i][j].id - 1] : matrix[sol.pistas[i][j-1].id - 1][sol.pistas[i][j].id - 1];
                if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                t += sol.pistas[i][j].duracao;
            }

            // std::cout << "Multa: " << multa << std::endl;
            // std::cout << "Antes: " << menor_multa << " Depois: " << multa << std::endl;
            if(multa < menor_multa){
                menor_multa = multa;
                pos_inicial = start;
            }
        }

        if(pos_inicial != -1){
            voo aux = sol.pistas[i][pos_inicial];
            sol.pistas[i][pos_inicial] = sol.pistas[i][pos_inicial+3];
            sol.pistas[i][pos_inicial+3] = aux;
            aux = sol.pistas[i][pos_inicial+1];
            sol.pistas[i][pos_inicial+1] = sol.pistas[i][pos_inicial+2];
            sol.pistas[i][pos_inicial+2] = aux;

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }

    return sol;
}

solucao re_insertion(solucao sol, int** matriz, int num_pistas) {
    for(int i = 0; i < num_pistas; i++) {
        int tamanho = sol.pistas[i].size();
        
        if(tamanho < 2) {
            continue; 
        }

        int menor_multa = 0;
        for(voo v : sol.pistas[i]) {
            menor_multa += sol.voos_multa[v.id-1];
        }

        int multa_antes = menor_multa;
        int melhor_pos_mover = -1;
        int melhor_nova_pos = -1;

        for(int pos_mover = 0; pos_mover < tamanho; pos_mover++) {
            for(int nova_pos = 0; nova_pos < tamanho; nova_pos++) {
                if(nova_pos == pos_mover || nova_pos == pos_mover + 1) {
                    continue;
                }

                int multa = 0;
                int tempo = 0;
                std::vector<voo> pista_temp;

                if(nova_pos < pos_mover) {
                    for(int j = 0; j < nova_pos; j++) {
                        if(j != pos_mover) pista_temp.push_back(sol.pistas[i][j]);
                    }
                    pista_temp.push_back(sol.pistas[i][pos_mover]);
                    for(int j = nova_pos; j < tamanho; j++) {
                        if(j != pos_mover) pista_temp.push_back(sol.pistas[i][j]);
                    }
                } else {
                    for(int j = 0; j < pos_mover; j++) {
                        pista_temp.push_back(sol.pistas[i][j]);
                    }
                    for(int j = pos_mover + 1; j < nova_pos; j++) {
                        pista_temp.push_back(sol.pistas[i][j]);
                    }
                    pista_temp.push_back(sol.pistas[i][pos_mover]);
                    for(int j = nova_pos; j < tamanho; j++) {
                        pista_temp.push_back(sol.pistas[i][j]);
                    }
                }

                for(int j = 0; j < tamanho; j++) {
                    if(j > 0) {
                        tempo += matriz[pista_temp[j-1].id - 1][pista_temp[j].id - 1];
                    }
                    if(tempo < pista_temp[j].t_decolagem) {
                        tempo += pista_temp[j].t_decolagem - tempo;
                    } else if(tempo > pista_temp[j].t_decolagem) {
                        multa += pista_temp[j].multa * (tempo - pista_temp[j].t_decolagem);
                    }
                    tempo += pista_temp[j].duracao;
                }

                if(multa < menor_multa) {
                    menor_multa = multa;
                    melhor_pos_mover = pos_mover;
                    melhor_nova_pos = nova_pos;
                }
            }
        }

        if(melhor_pos_mover != -1) {
            voo voo_movido = sol.pistas[i][melhor_pos_mover];
            sol.pistas[i].erase(sol.pistas[i].begin() + melhor_pos_mover);
            
            if(melhor_nova_pos > melhor_pos_mover) {
                sol.pistas[i].insert(sol.pistas[i].begin() + melhor_nova_pos - 1, voo_movido);
            } else {
                sol.pistas[i].insert(sol.pistas[i].begin() + melhor_nova_pos, voo_movido);
            }

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }

    return sol;
}