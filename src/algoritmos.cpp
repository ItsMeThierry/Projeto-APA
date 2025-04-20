#include "include/algoritmos.h"

solucao algoritmo_guloso(solucao sol, voo* voos, int** matrix, int num_voos, int num_pistas){

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

void two_opt(solucao &sol, int**matrix, int num_pistas){
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
            for(int c = 0; pos_inicial+c < pos_final-c; c++){
                aux = sol.pistas[i][pos_inicial+c];
                sol.pistas[i][pos_inicial+c] = sol.pistas[i][pos_final-1-c];
                sol.pistas[i][pos_final-1-c] = aux;
            }

            sol.multa -= multa_antes;
            sol.multa += menor_multa;
        }
    }
}

void re_insertion(solucao &sol, int** matriz, int num_pistas) {
    for(int i = 0; i < num_pistas; i++) {
        int tamanho = sol.pistas[i].size();
        
        if(tamanho < 2) {
            continue; 
        }

        int menor_multa = 0;

        for(int v = 0, t = 0; v < tamanho; v++){
            if(v > 0) t += matriz[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
            if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
            else if(t > sol.pistas[i][v].t_decolagem){ menor_multa += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem); }
            t += sol.pistas[i][v].duracao;
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
}

void swap(solucao &sol, int**matrix, int num_pistas) {
    for(int i = 0; i < num_pistas; i++){
        int size = sol.pistas[i].size();

        if(size == 1){
            continue;
        }

        int multa_antes = calcula_multa_pista(sol.pistas[i], matrix);
        int multaAtual = multa_antes;

        bool melhorou = true;
        std::vector<voo> melhorSequencia = sol.pistas[i];

        while (melhorou) {
            melhorou = false;
            for (int j = 0; j < size - 1; j++) {
                for (int k = j+1; k < size; k++){
                    std::vector<voo> novaSequencia = sol.pistas[i];
                    std::swap(novaSequencia[j], novaSequencia[k]);

                    int novaMulta = calcula_multa_pista(novaSequencia, matrix);

                    if (novaMulta < multaAtual) {
                        melhorSequencia.empty();
                        melhorSequencia = novaSequencia;
                        multaAtual = novaMulta;
                        melhorou = true;
                        break;
                    }
                }
            }
        }

        sol.multa -= multa_antes;
        sol.multa += multaAtual;
        sol.pistas[i] = melhorSequencia;
    }

}

void swap_pistas(solucao &sol, int**matrix, int num_pistas){

    // Cria um array auxiliar para indicar pistas que já sofreram swap
    bool swaped[num_pistas];

    for(int i = 0; i < num_pistas; i++){
        swaped[i] = false;
    }

    // Pra cada pista, seleciona pra ser a 1ª pista selecionada pro swap
    for(int i = 0; i < num_pistas; i++){
        if(swaped[i]){
            continue;
        }

        int size_p1 = sol.pistas[i].size();
        int multa_p1_antes = calcula_multa_pista(sol.pistas[i], matrix);

        int multa_original = 0; // Como era a soma da multa das duas pistas antes do swap
        int p2 = -1; // O indice da 2ª pista selecionada pro swap
        int v1 = -1; // O indice do voo da 1ª pista selecionada pro swap
        int v2 = -1; // O indice do voo da 2ª pista selecionada pro swap
        int menor_multa = INT_MAX; // Menor multa possivel

        // Pra cada voo selecionado na 1ª pista selecionada
        for(int pos_1 = 0; pos_1 < size_p1; pos_1++){
            // Pra cada outra pista, seleciona pra ser a 2ª pista selecionada pro swap
            for(int j = 0; j < num_pistas; j++){
                if(swaped[j] || j == i){
                    continue;
                }

                int size_p2 = sol.pistas[j].size();

                // Calcula como era a multa antes na 2ª pista selecionada
                int multa_p2_antes = calcula_multa_pista(sol.pistas[j], matrix);

                // Pra cada voo k na 2ª pista selecionada
                for(int pos_2 = 0; pos_2 < size_p2; pos_2++){
                    int m = 0;

                    // Calcula a multa do swap na 1ª pista selecionada
                    for(int v = 0, t = 0; v < size_p1; v++){
                        if(v != pos_1){
                            if(v > 0) t += (v-1 == pos_1) ? matrix[sol.pistas[j][pos_2].id - 1][sol.pistas[i][v].id - 1] : matrix[sol.pistas[i][v-1].id - 1][sol.pistas[i][v].id - 1];
                            if(t < sol.pistas[i][v].t_decolagem){ t += sol.pistas[i][v].t_decolagem - t; }
                            else if(t > sol.pistas[i][v].t_decolagem){ m += sol.pistas[i][v].multa * (t - sol.pistas[i][v].t_decolagem); }
                            t += sol.pistas[i][v].duracao;
                        } else{
                            if(v > 0) t += matrix[sol.pistas[i][v-1].id - 1][sol.pistas[j][pos_2].id - 1];
                            if(t < sol.pistas[j][pos_2].t_decolagem){ t += sol.pistas[j][pos_2].t_decolagem - t; }
                            else if(t > sol.pistas[j][pos_2].t_decolagem){ m += sol.pistas[j][pos_2].multa * (t - sol.pistas[j][pos_2].t_decolagem);}
                            t += sol.pistas[j][pos_2].duracao;
                        }
                    }

                    // Calcula a multa do swap na 2ª pista selecionada
                    for(int v = 0, t = 0; v < size_p2; v++){
                        if(v != pos_2){
                            if(v > 0) t += (v-1 == pos_2) ? matrix[sol.pistas[i][pos_1].id - 1][sol.pistas[j][v].id - 1] : matrix[sol.pistas[j][v-1].id - 1][sol.pistas[j][v].id - 1];
                            if(t < sol.pistas[j][v].t_decolagem){ t += sol.pistas[j][v].t_decolagem - t; }
                            else if(t > sol.pistas[j][v].t_decolagem){ m += sol.pistas[j][v].multa * (t - sol.pistas[j][v].t_decolagem); }
                            t += sol.pistas[j][v].duracao;
                        } else{
                            if(v > 0) t += matrix[sol.pistas[j][v-1].id - 1][sol.pistas[i][pos_1].id - 1];
                            if(t < sol.pistas[i][pos_1].t_decolagem){ t += sol.pistas[i][pos_1].t_decolagem - t; }
                            else if(t > sol.pistas[i][pos_1].t_decolagem){ m += sol.pistas[i][pos_1].multa * (t - sol.pistas[i][pos_1].t_decolagem); }
                            t += sol.pistas[i][pos_1].duracao;
                        }
                    }

                    // Se a multa do swap for a menor encontrada pra solução, salva os dados para realizar o swap
                    if(m < menor_multa){
                        menor_multa = m;
                        v2 = pos_2;
                        v1 = pos_1;
                        p2 = j;
                        multa_original = multa_p1_antes + multa_p2_antes;
                    }
                }
            }
        }

        // Realiza o swap de fato
        if(v1 != -1 && v2 != -1){
            std::swap(sol.pistas[i][v1], sol.pistas[p2][v2]);
            sol.multa -= multa_original;
            sol.multa += menor_multa;
            swaped[p2] = true;
        }

        swaped[i] = true;
    }

}

void re_insertion2(solucao &sol, int** matriz, int num_pistas) {
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

solucao vnd(solucao otimo, int**matrix, int num_pistas){
    int k = 1;
    int menor_multa = otimo.multa;

    while(k <= 5){
        switch(k){
            case 1:
                re_insertion2(otimo, matrix, num_pistas);
                break;
            case 2:
                swap_pistas(otimo, matrix, num_pistas);
                break;
            case 3:
                re_insertion(otimo, matrix, num_pistas);
                break;
            case 4:
                swap(otimo, matrix, num_pistas);
                break;
            case 5:
                two_opt(otimo, matrix, num_pistas);
                break;
        }

        if(otimo.multa < menor_multa){
            menor_multa = otimo.multa;
            k = 1;
        }else{
            k++;
        }
    }

    return otimo;
}

solucao ils(solucao s, int** matrix, int num_pistas){
    int tentativa = 1;
    while(tentativa < num_pistas){
        solucao sp = pertubacao(s, tentativa, matrix, num_pistas);
        solucao sp_ = vnd(sp, matrix, num_pistas);

        if(sp_.multa < s.multa){
            s = sp_;
        } else{
            tentativa++;
        }

        if(s.multa < 0){
            break;
        }
    }

    return s;
}

solucao pertubacao(solucao sol, int shift, int**matrix, int num_pistas){
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
        int size = voos_deslocados[(i+shift+1) % num_pistas].size();

        for(int j = size - 1; j >= 0; j--){
            sol.pistas[i].insert(sol.pistas[i].begin(), voos_deslocados[(i+shift+1) % num_pistas][j]);
        }
    }

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
    
    return sol;
}