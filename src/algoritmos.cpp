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
    std::srand(std::time(0));
    int pista = std::rand() % num_pistas;
    int pos_1 = std::rand() % ((int) sol.pistas[pista].size());
    int pos_2 = pos_1;

    while(pos_2 == pos_1){
        pos_2 = std::rand() % ((int) sol.pistas[pista].size());
    }

    std::swap(sol.pistas[pista][pos_1], sol.pistas[pista][pos_2]);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void swap_pistas(solucao &sol, int**matrix, int num_pistas){
    std::srand(std::time(0));
    int pista_1 = std::rand() % num_pistas;
    int pista_2 = pista_1;

    while(pista_2 == pista_1){
        pista_2 = std::rand() % num_pistas;
    }

    int pos_1 = std::rand() % ((int) sol.pistas[pista_1].size());
    int pos_2 = std::rand() % ((int) sol.pistas[pista_2].size());

    std::swap(sol.pistas[pista_1][pos_1], sol.pistas[pista_2][pos_2]);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void re_insertion(solucao &sol, int** matrix, int num_pistas){
    std::srand(std::time(0));
    int pista = std::rand() % num_pistas;
    int pos_1 = std::rand() % ((int) sol.pistas[pista].size());

    voo voo_movido = sol.pistas[pista][pos_1];
    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1);

    int pos_2 = pos_1;

    while(pos_2 == pos_1){
        pos_2 = std::rand() % sol.pistas[pista].size();
    }

    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void re_insertion_pistas(solucao &sol, int** matrix, int num_pistas) {
    std::srand(std::time(0));
    int pista_1 = std::rand() % num_pistas;
    int pista_2 = pista_1;

    while(pista_2 == pista_1){
        pista_2 = std::rand() % num_pistas;
    }

    int pos_1 = std::rand() % ((int) sol.pistas[pista_1].size());
    int pos_2 = std::rand() % ((int) sol.pistas[pista_2].size());

    voo voo_movido = sol.pistas[pista_1][pos_1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1);

    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void re_insertion_2(solucao &sol, int**matrix, int num_pistas){
    std::srand(std::time(0));
    int pista = std::rand() % num_pistas;
    int pos_1 = std::rand() % ((int) sol.pistas[pista].size() - 1);

    voo voo_movido_1 = sol.pistas[pista][pos_1];
    voo voo_movido_2 = sol.pistas[pista][pos_1+1];
    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 1);

    int pos_2 = pos_1;

    while(pos_2 == pos_1){
        pos_2 = std::rand() % ((int) sol.pistas[pista].size());
    }

    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void re_insertion_2_pistas(solucao &sol, int**matrix, int num_pistas){
    std::srand(std::time(0));
    int pista_1 = std::rand() % num_pistas;
    int pista_2 = pista_1;

    while(pista_2 == pista_1){
        pista_2 = std::rand() % num_pistas;
    }

    int pos_1 = std::rand() % ((int) sol.pistas[pista_1].size() - 1);

    voo voo_movido_1 = sol.pistas[pista_1][pos_1];
    voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 1);

    int pos_2 = std::rand() % ((int) sol.pistas[pista_2].size());

    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);

    int multa = 0;
    for(int i = 0; i < num_pistas; i++){
        multa += calcula_multa_pista(sol.pistas[i], matrix);
    }

    sol.multa = multa;
}

void sa(solucao &otimo, int** matrix, int num_pistas) {
    long double temperatura_inicial = calcula_temperatura_inicial(otimo, matrix, num_pistas);
    long double temperatura = temperatura_inicial;
    float coeficiente = 0.99;
    int passo = 0;
    solucao s_atual = otimo;

    std::cout << "T0: " << temperatura_inicial << '\n';

    while(temperatura > (temperatura_inicial / 500) && passo < 1000){
        for(int l = 0; l < 5000; l++){
            solucao s_ = generate_neighbor(s_atual, matrix, num_pistas);
            int variacao = s_.multa - s_atual.multa;
            // std::cout << variacao << '\n';

            if(variacao <= 0){
                s_atual = s_;

                if(s_atual.multa < otimo.multa){
                    otimo = s_atual;
                    std::cout << otimo.multa << '\n';
                }
            } else{
                double r = static_cast<double>(std::rand()) / (RAND_MAX + 1);
                if(r < exp(-variacao/temperatura)){
                    s_atual = s_;
                }
            }
        }

        temperatura *= coeficiente;
        passo++;
    }
}

long double calcula_temperatura_inicial(solucao &sol, int**matrix, int num_pistas){
    double sum_variacao = 0;

    for(int i = 0; i < 1000; i++){
        solucao s = generate_neighbor(sol, matrix, num_pistas);

        sum_variacao += (double) abs(s.multa - sol.multa) / sol.multa;
    }

    double media = (double) sum_variacao / 1000;
    std::cout << media << '\n';

    return exp(media / 0.25);
}

solucao generate_neighbor(solucao sol, int**matrix, int num_pistas){
    int k = std::rand() % 6;

    switch(k) {
        case 0:
            // std::cout << "SWAP\n";
            swap(sol, matrix, num_pistas);
            break;
        case 1:
            // std::cout << "RE_INSERTION\n";
            re_insertion(sol, matrix, num_pistas);
            break;
        case 2:
            // std::cout << "SWAP_PISTAS\n";
            swap_pistas(sol, matrix, num_pistas);
            break;
        case 3:
            // std::cout << "RE_INSERTION_PISTAS\n";
            re_insertion_pistas(sol, matrix, num_pistas);
            break;
        case 4:
            // std::cout << "RE_INSERTION_2\n";
            re_insertion_2(sol, matrix, num_pistas);
            break;
        case 5:
            // std::cout << "RE_INSERTION_2_PISTAS\n";
            re_insertion_2_pistas(sol, matrix, num_pistas);
            break;
    }

    // std::cout << "Random multa: " << sol.multa << '\n';

    return sol;
}