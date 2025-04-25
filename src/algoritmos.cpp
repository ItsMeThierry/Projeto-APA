#include "include/algoritmos.h"

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

void swap(solucao &sol, int**matrix, int num_pistas) {
    int menor_multa = INT_MAX;
    std::vector<voo> melhorSequencia;
    int pista = -1;

    for(int i = 0; i < num_pistas; i++){
        int size = sol.pistas[i].size();

        if(size == 1){
            continue;
        }

        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++){
                if(j == k) continue;
                std::vector<voo> novaSequencia = sol.pistas[i];
                std::swap(novaSequencia[j], novaSequencia[k]);

                int novaMulta = calcula_multa_pista(novaSequencia, matrix);

                if (novaMulta < menor_multa) {
                    melhorSequencia = novaSequencia;
                    menor_multa = novaMulta;
                    pista = i;
                }
            }
        }
    }

    if(pista != - 1){
        int multa_antes = calcula_multa_pista(sol.pistas[pista], matrix);

        if(menor_multa < multa_antes){
            sol.multa -= multa_antes;
            sol.multa += menor_multa;
            sol.pistas[pista] = melhorSequencia;
        }
    }
}

void swap_pistas(solucao &sol, int**matrix, int num_pistas){
    int menor_multa = INT_MAX;
    int pista_origem = -1;
    int pista_destino = -1;
    int pos_origem = -1;
    int pos_destino = -1;

    for(int origem = 0; origem < num_pistas; origem++){
        int size_origem = sol.pistas[origem].size();

        for(int pos_1 = 0; pos_1 < size_origem; pos_1++){
            for(int destino = 0; destino < num_pistas; destino++){
                if(destino == origem) continue;
                int size_destino = sol.pistas[destino].size();

                for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                    int m = 0;

                    for(int v = 0, t = 0; v < size_origem; v++){
                        if(v != pos_1){
                            if(v > 0) t += (v-1 == pos_1) ? matrix[sol.pistas[destino][pos_2].id - 1][sol.pistas[origem][v].id - 1] : matrix[sol.pistas[origem][v-1].id - 1][sol.pistas[origem][v].id - 1];
                            if(t < sol.pistas[origem][v].t_decolagem){ t += sol.pistas[origem][v].t_decolagem - t; }
                            else if(t > sol.pistas[origem][v].t_decolagem){ m += sol.pistas[origem][v].multa * (t - sol.pistas[origem][v].t_decolagem); }
                            t += sol.pistas[origem][v].duracao;
                        } else{
                            if(v > 0) t += matrix[sol.pistas[origem][v-1].id - 1][sol.pistas[destino][pos_2].id - 1];
                            if(t < sol.pistas[destino][pos_2].t_decolagem){ t += sol.pistas[destino][pos_2].t_decolagem - t; }
                            else if(t > sol.pistas[destino][pos_2].t_decolagem){ m += sol.pistas[destino][pos_2].multa * (t - sol.pistas[destino][pos_2].t_decolagem);}
                            t += sol.pistas[destino][pos_2].duracao;
                        }
                    }

                    for(int v = 0, t = 0; v < size_destino; v++){
                        if(v != pos_2){
                            if(v > 0) t += (v-1 == pos_2) ? matrix[sol.pistas[origem][pos_1].id - 1][sol.pistas[destino][v].id - 1] : matrix[sol.pistas[destino][v-1].id - 1][sol.pistas[destino][v].id - 1];
                            if(t < sol.pistas[destino][v].t_decolagem){ t += sol.pistas[destino][v].t_decolagem - t; }
                            else if(t > sol.pistas[destino][v].t_decolagem){ m += sol.pistas[destino][v].multa * (t - sol.pistas[destino][v].t_decolagem); }
                            t += sol.pistas[destino][v].duracao;
                        } else{
                            if(v > 0) t += matrix[sol.pistas[destino][v-1].id - 1][sol.pistas[origem][pos_1].id - 1];
                            if(t < sol.pistas[origem][pos_1].t_decolagem){ t += sol.pistas[origem][pos_1].t_decolagem - t; }
                            else if(t > sol.pistas[origem][pos_1].t_decolagem){ m += sol.pistas[origem][pos_1].multa * (t - sol.pistas[origem][pos_1].t_decolagem); }
                            t += sol.pistas[origem][pos_1].duracao;
                        }
                    }

                    if(m < menor_multa){
                        menor_multa = m;
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
        int multa1_antes = calcula_multa_pista(sol.pistas[pista_origem], matrix);
        int multa2_antes = calcula_multa_pista(sol.pistas[pista_destino], matrix);

        if(menor_multa < multa1_antes+multa2_antes){
            std::swap(sol.pistas[pista_origem][pos_origem], sol.pistas[pista_destino][pos_destino]);
            sol.multa -= multa1_antes+multa2_antes;
            sol.multa += menor_multa;
        }
    }
}

void re_insertion(solucao &sol, int** matriz, int num_pistas){
    int menor_multa = INT_MAX;
    int pista = -1;
    int melhor_pos_mover = -1;
    int melhor_nova_pos = -1;

    for(int i = 0; i < num_pistas; i++) {
        int tamanho = sol.pistas[i].size();
        
        if(tamanho < 2) {
            continue; 
        }

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
                    pista = i;
                }
            }
        }
    }

    if(pista != -1) {
        int multa_antes = calcula_multa_pista(sol.pistas[pista], matriz);

        if(menor_multa < multa_antes){
            voo voo_movido = sol.pistas[pista][melhor_pos_mover];
            sol.pistas[pista].erase(sol.pistas[pista].begin() + melhor_pos_mover);
            
            if(melhor_nova_pos > melhor_pos_mover) {
                sol.pistas[pista].insert(sol.pistas[pista].begin() + melhor_nova_pos - 1, voo_movido);
            } else {
                sol.pistas[pista].insert(sol.pistas[pista].begin() + melhor_nova_pos, voo_movido);
            }

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }
}

void re_insertion_pistas(solucao &sol, int** matriz, int num_pistas) {
    int melhor_delta = 0;
    int melhor_pista_origem = -1, melhor_pos_mover = -1;
    int melhor_pista_destino = -1, melhor_nova_pos = -1;
    voo voo_movido;

    // Explorar todos os movimentos possíveis
    for (int origem = 0; origem < num_pistas; ++origem) {
        int tamanho_origem = sol.pistas[origem].size();
        if (tamanho_origem == 0) continue;

        for (int pos = 0; pos < tamanho_origem; ++pos) {
            // Calcular multa original da pista de origem
            int multa_origem_antes = calcula_multa_pista(sol.pistas[origem], matriz);
            
            // Simular remoção do voo
            std::vector<voo> nova_origem = sol.pistas[origem];
            voo_movido = nova_origem[pos];
            nova_origem.erase(nova_origem.begin() + pos);
            int multa_origem_depois = calcula_multa_pista(nova_origem, matriz);

            // Testar todas as pistas de destino
            for (int destino = 0; destino < num_pistas; ++destino) {
                int multa_destino_antes = (destino == origem) ? 
                    multa_origem_antes : 
                    calcula_multa_pista(sol.pistas[destino], matriz);

                // Testar todas as posições na pista de destino
                int max_pos = (destino == origem) ? 
                    nova_origem.size() : 
                    sol.pistas[destino].size();

                for (int nova_pos = 0; nova_pos <= max_pos; ++nova_pos) {
                    // Simular inserção
                    std::vector<voo> novo_destino = (destino == origem) ? 
                        nova_origem : 
                        sol.pistas[destino];
                    
                    novo_destino.insert(novo_destino.begin() + nova_pos, voo_movido);
                    int multa_destino_depois = calcula_multa_pista(novo_destino, matriz);

                    // Calcular delta total
                    int delta = (destino == origem)
                        ? (multa_destino_depois - multa_origem_antes)
                        : (multa_origem_depois + multa_destino_depois) - 
                          (multa_origem_antes + multa_destino_antes);

                    // Atualizar melhor movimento
                    if (delta < melhor_delta) {
                        melhor_delta = delta;
                        melhor_pista_origem = origem;
                        melhor_pos_mover = pos;
                        melhor_pista_destino = destino;
                        melhor_nova_pos = nova_pos;
                    }
                }
            }
        }
    }

    // Aplicar o melhor movimento encontrado
    if (melhor_delta < 0) {
        // Remover da origem
        voo_movido = sol.pistas[melhor_pista_origem][melhor_pos_mover];
        sol.pistas[melhor_pista_origem].erase(
            sol.pistas[melhor_pista_origem].begin() + melhor_pos_mover
        );

        // Inserir no destino
        sol.pistas[melhor_pista_destino].insert(
            sol.pistas[melhor_pista_destino].begin() + melhor_nova_pos,
            voo_movido
        );

        // Atualizar multa total
        sol.multa += melhor_delta;
    }
}

void re_insertion_2(solucao &sol, int**matrix, int num_pistas){
    int menor_multa = INT_MAX;
    std::vector<voo> pista_alterada;
    int pista_id = -1;

    for(int i = 0; i < num_pistas; i++){
        // std::cout << "PISTA " << i+1 << ": \n";
        int size = sol.pistas[i].size();

        if(size == 3) continue;       

        for(int pos_selected = 0; pos_selected < size - 1; pos_selected++){
            // std::cout << "SELECIONADOS: ";
            // std::cout << sol.pistas[i][pos_selected].id << " " << sol.pistas[i][pos_selected+1].id << '\n';

            std::vector<voo> sobra = sol.pistas[i];

            sobra.erase(sobra.begin() + pos_selected, sobra.begin() + pos_selected + 2);

            int new_size = sobra.size();

            for(int pos_insert = 0; pos_insert <= new_size; pos_insert++){
                if(pos_insert == pos_selected) continue;
                std::vector<voo> modificada = sobra;
                modificada.insert(modificada.begin() + pos_insert, sol.pistas[i][pos_selected]);
                modificada.insert(modificada.begin() + pos_insert + 1, sol.pistas[i][pos_selected+1]);

                // std::cout << "pos_selected = " << pos_selected << " pos_insert = " << pos_insert << '\n';
                // for(voo v : modificada){
                //     std::cout << v.id << " ";
                // }
                // std::cout << '\n';

                int multa = calcula_multa_pista(modificada, matrix);

                if(multa < menor_multa){
                    menor_multa = multa;
                    pista_id = i;
                    pista_alterada = modificada;
                }
            }

        }
    }

    if(pista_id != -1){
        int multa_antes = calcula_multa_pista(sol.pistas[pista_id], matrix);

        if(menor_multa < multa_antes){
            sol.pistas[pista_id] = pista_alterada;
            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }
}

void re_insertion_2_pistas(solucao &sol, int**matrix, int num_pistas){
    int menor_multa = INT_MAX;
    std::vector<voo> pista_origem_alterada;
    std::vector<voo> pista_destino_alterada;
    int pista_destino = -1;
    int pista_origem = -1;

    for(int i = 0; i < num_pistas; i++){
        int size = sol.pistas[i].size();

        if(size == 3) continue;       

        for(int pos_selected = 0; pos_selected < size - 1; pos_selected++){

            std::vector<voo> pista_sobra = sol.pistas[i];

            pista_sobra.erase(pista_sobra.begin() + pos_selected, pista_sobra.begin() + pos_selected + 2);

            int multa_origem = calcula_multa_pista(pista_sobra, matrix);

            for(int destino = 0; destino < num_pistas; destino++){
                if(destino == i) continue;
                int destino_size = sol.pistas[destino].size();

                for(int pos_insert = 0; pos_insert <= destino_size; pos_insert++){
                    std::vector<voo> modificada = sol.pistas[destino];
                    modificada.insert(modificada.begin() + pos_insert, sol.pistas[i][pos_selected]);
                    modificada.insert(modificada.begin() + pos_insert + 1, sol.pistas[i][pos_selected+1]);

                    int multa_destino = calcula_multa_pista(modificada, matrix);

                    if(multa_origem+multa_destino < menor_multa){
                        menor_multa = multa_origem + multa_destino;
                        pista_origem = i;
                        pista_destino = destino;
                        pista_origem_alterada = pista_sobra;
                        pista_destino_alterada = modificada;
                    }
                }
            }

        }
    }

    if(pista_destino != -1){
        int multa_origem_antes = calcula_multa_pista(sol.pistas[pista_origem], matrix);
        int multa_destino_antes = calcula_multa_pista(sol.pistas[pista_destino], matrix);

        if(menor_multa < multa_origem_antes + multa_destino_antes){
            sol.pistas[pista_origem] = pista_origem_alterada;
            sol.pistas[pista_destino] = pista_destino_alterada;
            sol.multa -= multa_origem_antes + multa_destino_antes;
            sol.multa += menor_multa;
        }
    }
}

void vnd(solucao &otimo, int**matrix, int num_pistas){
    int k = 1;
    int menor_multa = otimo.multa;

    while(k <= 6){
        switch(k){
            case 1:
                swap(otimo, matrix, num_pistas);
                break;
            case 2:
                re_insertion(otimo, matrix, num_pistas);
                break;
            case 3:
                swap_pistas(otimo, matrix, num_pistas);
                break;
            case 4:
                re_insertion_pistas(otimo, matrix, num_pistas);
                break;
            case 5:
                re_insertion_2(otimo, matrix, num_pistas);
                break;
            case 6:
                re_insertion_2_pistas(otimo, matrix, num_pistas);
                break;
        }

        if(otimo.multa < menor_multa){
            menor_multa = otimo.multa;
            // std::cout << menor_multa << std::endl;
            k = 1;
        }else{
            k++;
        }
    }
}

void ils(solucao &s, int** matrix, int num_pistas){
    int tentativa = 1;
    solucao melhor_s = s;

    while(tentativa < num_pistas){
        solucao s_ = pertubacao(s, matrix, num_pistas);
        vnd(s_, matrix, num_pistas);

        if(s_.multa < melhor_s.multa){
            // std::cout << s_.multa << std::endl;
            melhor_s = s_;
            s = melhor_s;
            tentativa = 1;
        } else{
            tentativa++;
            s = s_;
        }
    }

    s = melhor_s;
}

solucao pertubacao(solucao sol, int**matrix, int num_pistas){
    if(num_pistas == 1){
        return sol;
    }

    std::vector<voo> voos_deslocados[num_pistas];

    for(int i = 0; i < num_pistas; i++){
        for(int j = 0; j < 3; j++){
            if(sol.pistas[i].size() > 0){
                voos_deslocados[i].push_back(sol.pistas[i].front());
                sol.pistas[i].erase(sol.pistas[i].begin());
            }
        }
    }

    for(int i = 0; i < num_pistas; i++){
        int size = voos_deslocados[(i+1) % num_pistas].size();

        for(int j = size - 1; j >= 0; j--){
            sol.pistas[i].insert(sol.pistas[i].begin(), voos_deslocados[(i+1) % num_pistas][j]);
        }
    }

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
    
    return sol;
}