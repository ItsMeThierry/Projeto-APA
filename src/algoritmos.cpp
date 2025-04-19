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

        int multa_antes = 0;

        for(int v = 0, t = 0; v < size; v++){
            if(v > 0) t += matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
            if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
            else if(t > sol.pistas[i][v].t_decolagem){ multa_antes += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem); }
            t += sol.pistas[i][v].duracao;
        }

        int menor_multa = multa_antes;
        int pos_inicial = -1;
        int pos_final = -1;

        for(int k = 4; k <= size; k++){
            for(int start = 0; start + (k-1) < size; start++){
                int multa = 0;
                int t = 0;

                for(int j = 0; j < start; j++){
                    if(j > 0) t += matrix[sol.pistas[i][j-1].id - 1][sol.pistas[i][j].id - 1];
                    if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                    else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                    t += sol.pistas[i][j].duracao;
                }

                for(int j = start + (k-1); j >= 0; j--){
                    if(t > 0) t += (j - (k-1) == start) ? matrix[sol.pistas[i][start - 1].id - 1][sol.pistas[i][j].id - 1] : matrix[sol.pistas[i][j+1].id - 1][sol.pistas[i][j].id - 1];
                    if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                    else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                    t += sol.pistas[i][j].duracao;
                }

                for(int j = start + k; j < size; j++){
                    t += (j == start + k) ? matrix[sol.pistas[i][start].id - 1][sol.pistas[i][j].id - 1] : matrix[sol.pistas[i][j-1].id - 1][sol.pistas[i][j].id - 1];
                    if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t;}
                    else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem);}
                    t += sol.pistas[i][j].duracao;
                }

                if(multa < menor_multa){
                    menor_multa = multa;
                    pos_inicial = start;
                    pos_final = k;
                }
            }
        }

        if(pos_inicial != -1){
            voo aux;
            std::cout << pos_inicial << std::endl;
            std::cout << pos_final << std::endl;
            for(int c = 0; pos_inicial+c < pos_final-c; c++){
                aux = sol.pistas[i][pos_inicial+c];
                sol.pistas[i][pos_inicial+c] = sol.pistas[i][pos_final-1-c];
                sol.pistas[i][pos_final-1-c] = aux;
            }

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }

    return sol;
}

void re_insertion(solucao &sol, int** matrix, int num_pistas) {
    for(int i = 0; i < num_pistas; i++) {
        int size = sol.pistas[i].size();

        if (size < 2){
            continue;
        }

        int menor_multa = 0;

        for(int v = 0, t = 0; v < size; v++){
            if(v > 0) t += matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
            if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
            else if(t > sol.pistas[i][v].t_decolagem){ menor_multa += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem); }
            t += sol.pistas[i][v].duracao;
        }

        int multa_antes = menor_multa;
        int candidato = -1;
        int empurrado = -1;

        for(int j = size - 1; j >= 0; j--){
            for(int k = j - 1; k >= 0; k--){
                int multa = 0;
                int t = 0;

                for(int v = 0; v < k; v++){
                    if(v > 0) t += matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
                    if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
                    else if(t > sol.pistas[i][v].t_decolagem){ multa += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem); }
                    t += sol.pistas[i][v].duracao;
                }

                if (k > 0) t += matrix[sol.pistas[i][k-1].id - 1][sol.pistas[i][j].id - 1];
                if(t < sol.pistas[i][j].t_decolagem){ t += sol.pistas[i][j].t_decolagem - t; }
                else if(t > sol.pistas[i][j].t_decolagem){ multa += sol.pistas[i][j].multa * (t - sol.pistas[i][j].t_decolagem); }
                t += sol.pistas[i][j].duracao;

                for(int v = k; v < size; v++){
                    if(v == j){
                        continue;
                    }

                    if(v == k){
                        t += matrix[sol.pistas[i][j].id - 1][sol.pistas[i][v].id - 1];
                    } else if(v == j + 1){
                        t += matrix[sol.pistas[i][v-2].id - 1][sol.pistas[i][v].id - 1];
                    } else{
                        t += matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
                    }

                    if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t;}
                    else if(t > sol.pistas[i][v].t_decolagem){ multa += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem);}
                    t += sol.pistas[i][v].duracao;
                }

                if(multa < menor_multa){
                    menor_multa = multa;
                    candidato = j;
                    empurrado = k;
                }

            }
        }

        if(candidato != -1 && empurrado != -1){
            voo voo_transferido = sol.pistas[i][candidato];
            sol.pistas[i].erase(sol.pistas[i].begin() + candidato);
            sol.pistas[i].insert(sol.pistas[i].begin() + empurrado, voo_transferido);

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }
}

solucao vnd(solucao otimo, int**matrix, int num_pistas){
    int k = 1;
    int menor_multa = otimo.multa;

    while(k <= 2){
        switch(k){
            case 1:
                re_insertion(otimo, matrix, num_pistas);
                break;
            case 2:
                two_opt(otimo, matrix, num_pistas);
                break;
        }

        if(otimo.multa < menor_multa){
            menor_multa = otimo.multa;
            k = 1;
        }else{
            k++;
        }

        if(menor_multa < 0){
            break;
        }
    }

    return otimo;
}