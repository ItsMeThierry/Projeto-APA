#include "include/algoritmos.h"

thread_local static std::random_device rd;
thread_local static std::mt19937 gen(rd());

Dados CaixaPreta::dados;

Solucao CaixaPreta::algoritmo_guloso(){
    Solucao sol(dados.num_pistas);

    std::vector<int> fila_voos(dados.num_voos);
    std::iota(fila_voos.begin(), fila_voos.end(), 0); //O(n)
    std::sort(fila_voos.begin(), fila_voos.end(), [](int i1, int i2) {
        if (dados.array_decolagem[i1] == dados.array_decolagem[i2]) {
            return dados.array_penalidade[i1] > dados.array_penalidade[i2];
        }

        return dados.array_decolagem[i1] < dados.array_decolagem[i2];
    });
    
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

            if(!sol.pistas.at(pista).empty()) te = dados.matrix[sol.pistas.at(pista).back().id - 1][fila_voos[i]];
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
        sol.pistas.at(pista_inserida).push_back(v);
    }

    return sol;
}

int CaixaPreta::calcula_multa_pista(const std::vector<Voo> &pista, int pos_1, int pos_2){
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

void CaixaPreta::calcula_multa_pista(const std::vector<Voo> &pista, int &multa, int &t, int pos_1, int pos_2){
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

void CaixaPreta::atualiza_voos(std::vector<Voo> &pista, int pos_alterada){
    if(pista.empty()) return;

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

void CaixaPreta::vnd(Solucao &otimo){
    int k = 1;
    int menor_multa = otimo.multa;

    while(k <= 8){
        switch(k){
            case 1:
                swap(otimo);
                break;
            case 2:
                re_insertion_3_pistas(otimo);
                break;
            case 3:
                swap_pistas(otimo);
                break;
            case 4:
                re_insertion_3(otimo);
                break;
            case 5:
                re_insertion_2_pistas(otimo);
                break;
            case 6:
                re_insertion_2(otimo);
                break;
            case 7:
                re_insertion_pistas(otimo);
                break;
            case 8:
                re_insertion(otimo);
                break;
        }

        if(otimo.multa < menor_multa){
            menor_multa = otimo.multa;
            k = 1;
        }else{
            k++;
        }
    }
}

void CaixaPreta::ils(Solucao &s){
    struct Resultado{
        int menor_multa;
        int id_thread = -1;

        Resultado(const int valor) : menor_multa(valor){};
    };

    Resultado resultado(s.multa);
    int num_threads = 50;
    int tentativa = 0;

    std::mutex mtx;

    if(tentativa < 3){
        std::vector<std::thread> threads;
        std::barrier bar(num_threads);
        std::cout << "Criando threads...\n";

        for(int i = 0; i < num_threads; i++){
            threads.emplace_back([i, &s, &resultado, &mtx, &bar](){
                Solucao s_ = generate_neighbor(s);
                vnd(s_);

                mtx.lock();
                std::cout << "Thread " << i << " : multa = " << s_.multa << '\n';
                if(s_.multa < resultado.menor_multa){
                    resultado.menor_multa = s_.multa;
                    resultado.id_thread = i;
                    mtx.unlock();

                    bar.arrive_and_wait();

                    if(resultado.id_thread == i){
                        s = s_;
                    }

                    return;
                }
                mtx.unlock();
                bar.arrive_and_drop();
            });
        }

        for (auto& t : threads){
            t.join();
        }

        if(resultado.menor_multa == s.multa){
            tentativa++;
            std::cout << "tentativa " << tentativa+1 << '\n';
        } else{
            tentativa = 0;
        }
    }
}

void CaixaPreta::swap(Solucao &sol) {
    struct Resultado {
        int menor_multa = INT_MAX;
        int pista = -1;
        int pos_origem = -1;
        int pos_destino = -1;
    };

    Resultado resultado;
    std::mutex mtx;
    std::vector<std::thread> threads;

    for(int i = 0; i < dados.num_pistas; i++){
        threads.emplace_back([&sol, i, &resultado, &mtx](){
            Resultado resultado_thread;

            int size = sol.pistas[i].size();

            if(size < 2) return;

            for (int origem = 0; origem < size - 1; origem++) {
                for (int destino = origem + 1; destino < size; destino++){
                    int multa = (origem == 0) ? 0 : sol.pistas[i][origem - 1].multa_acumulada;
                    int t = (origem == 0) ? 0 : sol.pistas[i][origem - 1].t_decolagem + dados.array_duracao[sol.pistas[i][origem - 1].id - 1];

                    if(origem > 0) t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][destino].id - 1];
                    if(t < dados.array_decolagem[sol.pistas[i][destino].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino].id - 1] - t; }
                    else if(t > dados.array_decolagem[sol.pistas[i][destino].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino].id - 1]); }
                    t += dados.array_duracao[sol.pistas[i][destino].id - 1];

                    if(destino != origem + 1){
                        t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem + 1].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 1].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem + 1].id - 1];

                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 2, destino);

                        t += dados.matrix[sol.pistas[i][destino - 1].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
                    } else{
                        t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
                    }

                    if(destino != size - 1){
                        t += dados.matrix[sol.pistas[i][origem].id - 1][sol.pistas[i][destino + 1].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 1].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][destino + 1].id - 1];

                        calcula_multa_pista(sol.pistas[i], multa, t, destino + 2, size);
                    }
                    
                    if (multa < resultado_thread.menor_multa && multa < sol.multa_pistas[i]) {
                        resultado_thread.menor_multa = multa;
                        resultado_thread.pista = i;
                        resultado_thread.pos_origem = origem;
                        resultado_thread.pos_destino = destino;
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.menor_multa < resultado.menor_multa){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista != -1){
        std::swap(sol.pistas[resultado.pista][resultado.pos_origem], sol.pistas[resultado.pista][resultado.pos_destino]);

        atualiza_voos(sol.pistas[resultado.pista], resultado.pos_origem); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[resultado.pista];
        sol.multa += resultado.menor_multa;

        sol.multa_pistas[resultado.pista] = resultado.menor_multa;
    }
}

void CaixaPreta::swap_pistas(Solucao &sol){
    struct Resultado{
        int multa_origem = INT_MAX - 1;
        int multa_destino = 1;
        int pista_origem = -1;
        int pista_destino = -1;
        int pos_origem = -1;
        int pos_destino = -1;
    };

    Resultado resultado;
    std::mutex mtx;
    std::vector<std::thread> threads;

    for(int origem = 0; origem < dados.num_pistas - 1; origem++){
        threads.emplace_back([&sol, origem, &resultado, &mtx](){
            Resultado resultado_thread;

            int size_origem = sol.pistas[origem].size();

            for(int pos_1 = 0; pos_1 < size_origem; pos_1++){
                for(int destino = origem + 1; destino < dados.num_pistas; destino++){
                    
                    int size_destino = sol.pistas[destino].size();

                    for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                        int multa_1 = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].multa_acumulada;
                        int t_1 = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].t_decolagem + dados.array_duracao[sol.pistas[origem][pos_1 - 1].id - 1];

                        if(pos_1 > 0) t_1 += dados.matrix[sol.pistas[origem][pos_1 - 1].id - 1][sol.pistas[destino][pos_2].id - 1];
                        if(t_1 < dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ t_1 += dados.array_decolagem[sol.pistas[destino][pos_2].id - 1] - t_1; }
                        else if(t_1 > dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ multa_1 += dados.array_penalidade[sol.pistas[destino][pos_2].id - 1] * (t_1 - dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]); }
                        t_1 += dados.array_duracao[sol.pistas[destino][pos_2].id - 1];

                        if(pos_1 != size_origem - 1){
                            t_1 += dados.matrix[sol.pistas[destino][pos_2].id - 1][sol.pistas[origem][pos_1 + 1].id - 1];
                            if(t_1 < dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]){ t_1 += dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1] - t_1; }
                            else if(t_1 > dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]){ multa_1 += dados.array_penalidade[sol.pistas[origem][pos_1 + 1].id - 1] * (t_1 - dados.array_decolagem[sol.pistas[origem][pos_1 + 1].id - 1]); }
                            t_1 += dados.array_duracao[sol.pistas[origem][pos_1 + 1].id - 1];

                            calcula_multa_pista(sol.pistas[origem], multa_1, t_1, pos_1 + 2, size_origem);
                        }

                        int multa_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].multa_acumulada;
                        int t_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].t_decolagem + dados.array_duracao[sol.pistas[destino][pos_2 - 1].id - 1];

                        if(pos_2 > 0) t_2 += dados.matrix[sol.pistas[destino][pos_2 - 1].id - 1][sol.pistas[origem][pos_1].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[origem][pos_1].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ multa_2 += dados.array_penalidade[sol.pistas[origem][pos_1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[origem][pos_1].id - 1];

                        if(pos_2 != size_destino - 1){
                            t_2 += dados.matrix[sol.pistas[origem][pos_1].id - 1][sol.pistas[destino][pos_2 + 1].id - 1];
                            if(t_2 < dados.array_decolagem[sol.pistas[destino][pos_2 + 1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[destino][pos_2 + 1].id - 1] - t_2; }
                            else if(t_2 > dados.array_decolagem[sol.pistas[destino][pos_2 + 1].id - 1]){ multa_2 += dados.array_penalidade[sol.pistas[destino][pos_2 + 1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[destino][pos_2 + 1].id - 1]); }
                            t_2 += dados.array_duracao[sol.pistas[destino][pos_2 + 1].id - 1];

                            calcula_multa_pista(sol.pistas[destino], multa_2, t_2, pos_2 + 2, size_destino);
                        }

                        if(multa_1 + multa_2 < resultado_thread.multa_origem + resultado_thread.multa_destino && multa_1 + multa_2 < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                            resultado_thread.multa_origem = multa_1;
                            resultado_thread.multa_destino = multa_2;
                            resultado_thread.pista_origem = origem;
                            resultado_thread.pista_destino = destino;
                            resultado_thread.pos_origem = pos_1;
                            resultado_thread.pos_destino = pos_2;
                        }
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.multa_origem + resultado_thread.multa_destino < resultado.multa_origem + resultado.multa_destino){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista_origem != -1 && resultado.pista_destino != -1){
        std::swap(sol.pistas[resultado.pista_origem][resultado.pos_origem], sol.pistas[resultado.pista_destino][resultado.pos_destino]);

        atualiza_voos(sol.pistas[resultado.pista_origem], resultado.pos_origem); //O(n) no pior caso
        atualiza_voos(sol.pistas[resultado.pista_destino], resultado.pos_destino); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[resultado.pista_origem] + sol.multa_pistas[resultado.pista_destino];
        sol.multa += resultado.multa_origem + resultado.multa_destino;

        sol.multa_pistas[resultado.pista_origem] = resultado.multa_origem;
        sol.multa_pistas[resultado.pista_destino] = resultado.multa_destino;
    }
}

void CaixaPreta::re_insertion(Solucao &sol) {
    struct Resultado{
        int menor_multa = INT_MAX;
        int pista = -1;
        int pos_origem = -1;
        int pos_destino = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int i = 0; i < dados.num_pistas; i++){
        threads.emplace_back([&sol, i, &resultado, &mtx](){
            Resultado resultado_thread;
            int size = sol.pistas[i].size();

            if (size < 3) return;

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

                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 2, destino + 1);

                        t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];

                        if(destino != size - 1){
                            t += dados.matrix[sol.pistas[i][origem].id - 1][sol.pistas[i][destino + 1].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 1].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][destino + 1].id - 1];

                            calcula_multa_pista(sol.pistas[i], multa, t, destino + 2, size);
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

                        calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, origem);


                        if(origem != size - 1){
                            t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 1].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 1].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 1].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][origem + 1].id - 1];

                            calcula_multa_pista(sol.pistas[i], multa, t, origem + 2, size);
                        }
                    }

                    if(multa < resultado_thread.menor_multa && multa < sol.multa_pistas[i]){
                        resultado_thread.menor_multa = multa;
                        resultado_thread.pista = i;
                        resultado_thread.pos_origem = origem;
                        resultado_thread.pos_destino = destino;
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.menor_multa < resultado.menor_multa){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista != -1){
        Voo voo_realocado = sol.pistas[resultado.pista][resultado.pos_origem];
        sol.pistas[resultado.pista].erase(sol.pistas[resultado.pista].begin() + resultado.pos_origem);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino, voo_realocado);

        if(resultado.pos_origem < resultado.pos_destino){
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_origem); //O(n) no pior caso
        } else{
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_destino); //O(n) no pior caso
        }
        
        sol.multa -= sol.multa_pistas[resultado.pista];
        sol.multa += resultado.menor_multa;

        sol.multa_pistas[resultado.pista] = resultado.menor_multa;
    }
}

void CaixaPreta::re_insertion_pistas(Solucao &sol) {
    struct Resultado{
        int multa_origem = INT_MAX - 1;
        int multa_destino = 1;
        int pista_origem = -1;
        int pos_mover = -1;
        int pista_destino = -1;
        int nova_pos = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int origem = 0; origem < dados.num_pistas; origem++){
        threads.emplace_back([&sol, origem, &resultado, &mtx](){
            Resultado resultado_thread;
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

                            calcula_multa_pista(sol.pistas[origem], multa_origem, t_1, pos_1 + 2, size_origem);
                        }

                        // Calcula inserção em pista destino
                        int multa_destino = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].multa_acumulada;
                        int t_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].t_decolagem + dados.array_duracao[sol.pistas[destino][pos_2 - 1].id - 1];

                        if(pos_2 > 0) t_2 += dados.matrix[sol.pistas[destino][pos_2 - 1].id - 1][sol.pistas[origem][pos_1].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[origem][pos_1].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[origem][pos_1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[origem][pos_1].id - 1];

                        t_2 += dados.matrix[sol.pistas[origem][pos_1].id - 1][sol.pistas[destino][pos_2].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[destino][pos_2].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[destino][pos_2].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[destino][pos_2].id - 1];

                        calcula_multa_pista(sol.pistas[destino], multa_destino, t_2, pos_2 + 1, size_destino);
                            

                        if(multa_origem + multa_destino < resultado_thread.multa_origem + resultado_thread.multa_destino && multa_origem + multa_destino < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                            resultado_thread.multa_origem = multa_origem;
                            resultado_thread.multa_destino = multa_destino;
                            resultado_thread.pista_origem = origem;
                            resultado_thread.pista_destino = destino;
                            resultado_thread.pos_mover = pos_1;
                            resultado_thread.nova_pos = pos_2;
                        }
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.multa_origem + resultado_thread.multa_destino < resultado.multa_origem + resultado.multa_destino){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista_origem != -1 && resultado.pista_destino != -1){
        Voo voo_realocado = sol.pistas[resultado.pista_origem][resultado.pos_mover];
        sol.pistas[resultado.pista_origem].erase(sol.pistas[resultado.pista_origem].begin() + resultado.pos_mover);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos, voo_realocado);

        atualiza_voos(sol.pistas[resultado.pista_origem], resultado.pos_mover); //O(n) no pior caso
        atualiza_voos(sol.pistas[resultado.pista_destino], resultado.nova_pos); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[resultado.pista_origem] + sol.multa_pistas[resultado.pista_destino];
        sol.multa += resultado.multa_origem + resultado.multa_destino;

        sol.multa_pistas[resultado.pista_origem] = resultado.multa_origem;
        sol.multa_pistas[resultado.pista_destino] = resultado.multa_destino;
    }

}

void CaixaPreta::re_insertion_2(Solucao &sol){
    struct Resultado{
        int menor_multa = INT_MAX;
        int pista = -1;
        int pos_origem = -1;
        int pos_destino = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int i = 0; i < dados.num_pistas; i++){
        threads.emplace_back([&sol, i, &resultado, &mtx](){
            Resultado resultado_thread;
            int size = sol.pistas[i].size();

            if (size < 4) return;
    
            for(int origem = 0; origem < size - 1; origem++){
                for(int destino = 0; destino < size; destino++){
                    if(destino >= origem - 1 && destino <= origem + 2) continue;
                    int multa = 0;
    
                    if(origem < destino){
                        multa = (origem == 0) ? 0 : sol.pistas[i][origem - 1].multa_acumulada;
                        int t = (origem == 0) ? 0 : sol.pistas[i][origem - 1].t_decolagem + dados.array_duracao[sol.pistas[i][origem - 1].id - 1];
        
                        if(origem > 0) t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 2].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 2].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 2].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem + 2].id - 1];
        
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 3, destino + 1);
    
                        t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
    
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 2);
    
                        if(destino != size - 1){
                            t += dados.matrix[sol.pistas[i][origem + 1].id - 1][sol.pistas[i][destino + 1].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 1].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][destino + 1].id - 1];
    
                            calcula_multa_pista(sol.pistas[i], multa, t, destino + 2, size);
                        }
    
                    } else{
                        multa = (destino == 0) ? 0 : sol.pistas[i][destino - 1].multa_acumulada;
                        int t = (destino == 0) ? 0 : sol.pistas[i][destino - 1].t_decolagem + dados.array_duracao[sol.pistas[i][destino - 1].id - 1];
    
                        if(destino > 0) t += dados.matrix[sol.pistas[i][destino-1].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 2);
    
                        t += dados.matrix[sol.pistas[i][origem + 1].id - 1][sol.pistas[i][destino].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][destino].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][destino].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][destino].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, origem);
    
                        if(origem != size - 2){
                            t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 2].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 2].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 2].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 2].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][origem + 2].id - 1];
                            
                            calcula_multa_pista(sol.pistas[i], multa, t, origem + 3, size);
                        }
                    }
    
                    if(multa < resultado_thread.menor_multa && multa < sol.multa_pistas[i]){
                        resultado_thread.menor_multa = multa;
                        resultado_thread.pista = i;
                        resultado_thread.pos_origem = origem;
                        resultado_thread.pos_destino = destino;
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.menor_multa < resultado.menor_multa){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista != -1){
        Voo voo_realocado_1 = sol.pistas[resultado.pista][resultado.pos_origem];
        Voo voo_realocado_2 = sol.pistas[resultado.pista][resultado.pos_origem+1];

        if (resultado.pos_origem < resultado.pos_destino) {
            resultado.pos_destino -= 1;
        }

        sol.pistas[resultado.pista].erase(sol.pistas[resultado.pista].begin() + resultado.pos_origem, sol.pistas[resultado.pista].begin() + resultado.pos_origem + 2);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino, voo_realocado_1);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino + 1, voo_realocado_2);

        if(resultado.pos_origem < resultado.pos_destino){
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_origem); //O(n) no pior caso
        } else{
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_destino); //O(n) no pior caso
        }
            
        sol.multa -= sol.multa_pistas[resultado.pista];
        sol.multa += resultado.menor_multa;

        sol.multa_pistas[resultado.pista] = resultado.menor_multa;
    }
}

void CaixaPreta::re_insertion_2_pistas(Solucao &sol){
    struct Resultado{
        int multa_origem = INT_MAX - 1;
        int multa_destino = 1;
        int pista_origem = -1;
        int pos_mover = -1;
        int pista_destino = -1;
        int nova_pos = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int origem = 0; origem < dados.num_pistas; origem++){
        threads.emplace_back([&sol, origem, &resultado, &mtx](){
            Resultado resultado_thread;
            int size_origem = sol.pistas[origem].size();

            for(int pos_1 = 0; pos_1 < size_origem - 1; pos_1++){

                for(int destino = 0; destino < dados.num_pistas; destino++){
                    if(origem == destino) continue;
                    int size_destino = sol.pistas[destino].size();

                    for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                        // Calcula remoção em pista origem
                        int multa_origem = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].multa_acumulada;
                        int t_1 = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].t_decolagem + dados.array_duracao[sol.pistas[origem][pos_1 - 1].id - 1];

                        if(pos_1 != size_origem - 2){
                            if(pos_1 > 0) t_1 += dados.matrix[sol.pistas[origem][pos_1 - 1].id - 1][sol.pistas[origem][pos_1 + 2].id - 1];
                            if(t_1 < dados.array_decolagem[sol.pistas[origem][pos_1 + 2].id - 1]){ t_1 += dados.array_decolagem[sol.pistas[origem][pos_1 + 2].id - 1] - t_1; }
                            else if(t_1 > dados.array_decolagem[sol.pistas[origem][pos_1 + 2].id - 1]){ multa_origem += dados.array_penalidade[sol.pistas[origem][pos_1 + 2].id - 1] * (t_1 - dados.array_decolagem[sol.pistas[origem][pos_1 + 2].id - 1]); }
                            t_1 += dados.array_duracao[sol.pistas[origem][pos_1 + 2].id - 1];

                            calcula_multa_pista(sol.pistas[origem], multa_origem, t_1, pos_1 + 3, size_origem);
                        }

                        // Calcula inserção em pista destino
                        int multa_destino = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].multa_acumulada;
                        int t_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].t_decolagem + dados.array_duracao[sol.pistas[destino][pos_2 - 1].id - 1];

                        if(pos_2 > 0) t_2 += dados.matrix[sol.pistas[destino][pos_2 - 1].id - 1][sol.pistas[origem][pos_1].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[origem][pos_1].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[origem][pos_1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[origem][pos_1].id - 1];

                        calcula_multa_pista(sol.pistas[origem], multa_destino, t_2, pos_1 + 1, pos_1 + 2);

                        t_2 += dados.matrix[sol.pistas[origem][pos_1 + 1].id - 1][sol.pistas[destino][pos_2].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[destino][pos_2].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[destino][pos_2].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[destino][pos_2].id - 1];

                        calcula_multa_pista(sol.pistas[destino], multa_destino, t_2, pos_2 + 1, size_destino);
                            
                        if(multa_origem + multa_destino < resultado_thread.multa_origem + resultado_thread.multa_destino && multa_origem + multa_destino < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                            resultado_thread.multa_origem = multa_origem;
                            resultado_thread.multa_destino = multa_destino;
                            resultado_thread.pista_origem = origem;
                            resultado_thread.pista_destino = destino;
                            resultado_thread.pos_mover = pos_1;
                            resultado_thread.nova_pos = pos_2;
                        }
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.multa_origem + resultado_thread.multa_destino < resultado.multa_origem + resultado.multa_destino){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista_origem != -1 && resultado.pista_destino != -1){
        Voo voo_realocado_1 = sol.pistas[resultado.pista_origem][resultado.pos_mover];
        Voo voo_realocado_2 = sol.pistas[resultado.pista_origem][resultado.pos_mover + 1];

        sol.pistas[resultado.pista_origem].erase(sol.pistas[resultado.pista_origem].begin() + resultado.pos_mover, sol.pistas[resultado.pista_origem].begin() + resultado.pos_mover + 2);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos, voo_realocado_1);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos + 1, voo_realocado_2);

        atualiza_voos(sol.pistas[resultado.pista_origem], resultado.pos_mover); //O(n) no pior caso
        atualiza_voos(sol.pistas[resultado.pista_destino], resultado.nova_pos); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[resultado.pista_origem] + sol.multa_pistas[resultado.pista_destino];
        sol.multa += resultado.multa_origem + resultado.multa_destino;

        sol.multa_pistas[resultado.pista_origem] = resultado.multa_origem;
        sol.multa_pistas[resultado.pista_destino] = resultado.multa_destino;
    }
}

void CaixaPreta::re_insertion_3(Solucao &sol){
    struct Resultado{
        int menor_multa = INT_MAX;
        int pista = -1;
        int pos_origem = -1;
        int pos_destino = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int i = 0; i < dados.num_pistas; i++){
        threads.emplace_back([&sol, i, &resultado, &mtx](){
            Resultado resultado_thread;
            int size = sol.pistas[i].size();

            if (size < 5) return;
    
            for(int origem = 0; origem < size - 2; origem++){
                for(int destino = 0; destino < size; destino++){
                    if(destino >= origem - 1 && destino <= origem + 3) continue;
                    int multa = 0;
    
                    if(origem < destino){
                        multa = (origem == 0) ? 0 : sol.pistas[i][origem - 1].multa_acumulada;
                        int t = (origem == 0) ? 0 : sol.pistas[i][origem - 1].t_decolagem + dados.array_duracao[sol.pistas[i][origem - 1].id - 1];
                        
                        if(origem > 0) t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 3].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 3].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 3].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem + 3].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 4, destino + 1);
                        
                        t += dados.matrix[sol.pistas[i][destino].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
    
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 3);
                        
                        if(destino != size - 1){
                            t += dados.matrix[sol.pistas[i][origem + 2].id - 1][sol.pistas[i][destino + 1].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino + 1].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino + 1].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino + 1].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][destino + 1].id - 1];
                            
                            if(destino + 1 < size) calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, size);
                        }
    
                    } else{
                        multa = (destino == 0) ? 0 : sol.pistas[i][destino - 1].multa_acumulada;
                        int t = (destino == 0) ? 0 : sol.pistas[i][destino - 1].t_decolagem + dados.array_duracao[sol.pistas[i][destino - 1].id - 1];
    
                        if(destino > 0) t += dados.matrix[sol.pistas[i][destino-1].id - 1][sol.pistas[i][origem].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][origem].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][origem].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][origem].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, origem + 1, origem + 3);
    
                        t += dados.matrix[sol.pistas[i][origem + 2].id - 1][sol.pistas[i][destino].id - 1];
                        if(t < dados.array_decolagem[sol.pistas[i][destino].id - 1]){ t += dados.array_decolagem[sol.pistas[i][destino].id - 1] - t; }
                        else if(t > dados.array_decolagem[sol.pistas[i][destino].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][destino].id - 1] * (t - dados.array_decolagem[sol.pistas[i][destino].id - 1]); }
                        t += dados.array_duracao[sol.pistas[i][destino].id - 1];
                        
                        calcula_multa_pista(sol.pistas[i], multa, t, destino + 1, origem);
    
                        if(origem != size - 3){
                            t += dados.matrix[sol.pistas[i][origem - 1].id - 1][sol.pistas[i][origem + 3].id - 1];
                            if(t < dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]){ t += dados.array_decolagem[sol.pistas[i][origem + 3].id - 1] - t; }
                            else if(t > dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]){ multa += dados.array_penalidade[sol.pistas[i][origem + 3].id - 1] * (t - dados.array_decolagem[sol.pistas[i][origem + 3].id - 1]); }
                            t += dados.array_duracao[sol.pistas[i][origem + 3].id - 1];
                            
                            calcula_multa_pista(sol.pistas[i], multa, t, origem + 4, size);
                        }
                    }
    
                    if(multa < resultado_thread.menor_multa && multa < sol.multa_pistas[i]){
                        resultado_thread.menor_multa = multa;
                        resultado_thread.pista = i;
                        resultado_thread.pos_origem = origem;
                        resultado_thread.pos_destino = destino;
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.menor_multa < resultado.menor_multa){
                resultado = resultado_thread;
            }
        });
    }

    for (auto& t : threads){
        t.join();
    }
    
    if(resultado.pista != -1){
        Voo voo_realocado_1 = sol.pistas[resultado.pista][resultado.pos_origem];
        Voo voo_realocado_2 = sol.pistas[resultado.pista][resultado.pos_origem+1];
        Voo voo_realocado_3 = sol.pistas[resultado.pista][resultado.pos_origem+2];

        if (resultado.pos_origem < resultado.pos_destino) {
            resultado.pos_destino -= 2;
        }

        sol.pistas[resultado.pista].erase(sol.pistas[resultado.pista].begin() + resultado.pos_origem, sol.pistas[resultado.pista].begin() + resultado.pos_origem + 3);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino, voo_realocado_1);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino + 1, voo_realocado_2);
        sol.pistas[resultado.pista].insert(sol.pistas[resultado.pista].begin() + resultado.pos_destino + 2, voo_realocado_3);

        if(resultado.pos_origem < resultado.pos_destino){
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_origem); //O(n) no pior caso
        } else{
            atualiza_voos(sol.pistas[resultado.pista], resultado.pos_destino); //O(n) no pior caso
        }
            
        sol.multa -= sol.multa_pistas[resultado.pista];
        sol.multa += resultado.menor_multa;

        sol.multa_pistas[resultado.pista] = resultado.menor_multa;
    }
}

void CaixaPreta::re_insertion_3_pistas(Solucao &sol){
    struct Resultado{
        int multa_origem = INT_MAX - 1;
        int multa_destino = 1;
        int pista_origem = -1;
        int pos_mover = -1;
        int pista_destino = -1;
        int nova_pos = -1;
    };

    Resultado resultado;
    std::vector<std::thread> threads;
    std::mutex mtx;

    for(int origem = 0; origem < dados.num_pistas; origem++){
        threads.emplace_back([&sol, origem, &resultado, &mtx](){
            Resultado resultado_thread;
            int size_origem = sol.pistas[origem].size();

            for(int pos_1 = 0; pos_1 < size_origem - 2; pos_1++){
    
                for(int destino = 0; destino < dados.num_pistas; destino++){
                    if(origem == destino) continue;
                    int size_destino = sol.pistas[destino].size();
    
                    for(int pos_2 = 0; pos_2 < size_destino; pos_2++){
                        // Calcula remoção em pista origem
                        int multa_origem = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].multa_acumulada;
                        int t_1 = (pos_1 == 0) ? 0 : sol.pistas[origem][pos_1 - 1].t_decolagem + dados.array_duracao[sol.pistas[origem][pos_1 - 1].id - 1];
    
                        if(pos_1 != size_origem - 3){
                            if(pos_1 > 0) t_1 += dados.matrix[sol.pistas[origem][pos_1 - 1].id - 1][sol.pistas[origem][pos_1 + 3].id - 1];
                            if(t_1 < dados.array_decolagem[sol.pistas[origem][pos_1 + 3].id - 1]){ t_1 += dados.array_decolagem[sol.pistas[origem][pos_1 + 3].id - 1] - t_1; }
                            else if(t_1 > dados.array_decolagem[sol.pistas[origem][pos_1 + 3].id - 1]){ multa_origem += dados.array_penalidade[sol.pistas[origem][pos_1 + 3].id - 1] * (t_1 - dados.array_decolagem[sol.pistas[origem][pos_1 + 3].id - 1]); }
                            t_1 += dados.array_duracao[sol.pistas[origem][pos_1 + 3].id - 1];
    
                            calcula_multa_pista(sol.pistas[origem], multa_origem, t_1, pos_1 + 4, size_origem);
                        }
    
                        // Calcula inserção em pista destino
                        int multa_destino = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].multa_acumulada;
                        int t_2 = (pos_2 == 0) ? 0 : sol.pistas[destino][pos_2 - 1].t_decolagem + dados.array_duracao[sol.pistas[destino][pos_2 - 1].id - 1];
    
                        if(pos_2 > 0) t_2 += dados.matrix[sol.pistas[destino][pos_2 - 1].id - 1][sol.pistas[origem][pos_1].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[origem][pos_1].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[origem][pos_1].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[origem][pos_1].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[origem][pos_1].id - 1];
    
                        calcula_multa_pista(sol.pistas[origem], multa_destino, t_2, pos_1 + 1, pos_1 + 3);
    
                        t_2 += dados.matrix[sol.pistas[origem][pos_1 + 2].id - 1][sol.pistas[destino][pos_2].id - 1];
                        if(t_2 < dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ t_2 += dados.array_decolagem[sol.pistas[destino][pos_2].id - 1] - t_2; }
                        else if(t_2 > dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]){ multa_destino += dados.array_penalidade[sol.pistas[destino][pos_2].id - 1] * (t_2 - dados.array_decolagem[sol.pistas[destino][pos_2].id - 1]); }
                        t_2 += dados.array_duracao[sol.pistas[destino][pos_2].id - 1];
    
                        calcula_multa_pista(sol.pistas[destino], multa_destino, t_2, pos_2 + 1, size_destino);
                            
                        if(multa_origem + multa_destino < resultado_thread.multa_origem + resultado_thread.multa_destino && multa_origem + multa_destino < sol.multa_pistas[origem] + sol.multa_pistas[destino]){
                            resultado_thread.multa_origem = multa_origem;
                            resultado_thread.multa_destino = multa_destino;
                            resultado_thread.pista_origem = origem;
                            resultado_thread.pista_destino = destino;
                            resultado_thread.pos_mover = pos_1;
                            resultado_thread.nova_pos = pos_2;
                        }
                    }
                }
            }

            std::lock_guard<std::mutex> lock(mtx);
            if(resultado_thread.multa_origem + resultado_thread.multa_destino < resultado.multa_origem + resultado.multa_destino){
                resultado = resultado_thread;
            }
        });
    }
    
    for (auto& t : threads){
        t.join();
    }

    if(resultado.pista_origem != -1 && resultado.pista_destino != -1){
        Voo voo_realocado_1 = sol.pistas[resultado.pista_origem][resultado.pos_mover];
        Voo voo_realocado_2 = sol.pistas[resultado.pista_origem][resultado.pos_mover + 1];
        Voo voo_realocado_3 = sol.pistas[resultado.pista_origem][resultado.pos_mover + 2];

        sol.pistas[resultado.pista_origem].erase(sol.pistas[resultado.pista_origem].begin() + resultado.pos_mover, sol.pistas[resultado.pista_origem].begin() + resultado.pos_mover + 3);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos, voo_realocado_1);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos + 1, voo_realocado_2);
        sol.pistas[resultado.pista_destino].insert(sol.pistas[resultado.pista_destino].begin() + resultado.nova_pos + 2, voo_realocado_3);

        atualiza_voos(sol.pistas[resultado.pista_origem], resultado.pos_mover); //O(n) no pior caso
        atualiza_voos(sol.pistas[resultado.pista_destino], resultado.nova_pos); //O(n) no pior caso

        sol.multa -= sol.multa_pistas[resultado.pista_origem] + sol.multa_pistas[resultado.pista_destino];
        sol.multa += resultado.multa_origem + resultado.multa_destino;

        sol.multa_pistas[resultado.pista_origem] = resultado.multa_origem;
        sol.multa_pistas[resultado.pista_destino] = resultado.multa_destino;
    }
}


void CaixaPreta::rand_swap(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    int pista = -1;
    
    for(int i = 0; i < 100; i++){
        pista = dist_pista(gen);

        if(sol.pistas[pista].size() >= 2){
            break;
        }
    }

    if (pista == -1 || sol.pistas[pista].size() < 2) return;

    std::uniform_int_distribution<int> dist_pos(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_1 = dist_pos(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista].size()) - 1) return;
    
    int pos_2 = pos_1;
    for(int i = 0; i < 100; i++){
        pos_2 = dist_pos(gen);

        if(pos_2 != pos_1){
            break;
        }
    }

    if(pos_2 == pos_1) return;

    std::swap(sol.pistas[pista][pos_1], sol.pistas[pista][pos_2]);

    if(pos_1 < pos_2){
        atualiza_voos(sol.pistas[pista], pos_1);
    } else{
        atualiza_voos(sol.pistas[pista], pos_2);
    }

    int multa_pista = sol.pistas[pista].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista];
    sol.multa += multa_pista;

    sol.multa_pistas[pista] = multa_pista;
}

void CaixaPreta::rand_swap_pistas(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista_1 = -1;
    for(int i = 0; i < 100; i++){
        pista_1 = dist_pista(gen);

        if(!sol.pistas[pista_1].empty()){
            break;
        }
    }

    if(pista_1 == -1 || sol.pistas[pista_1].empty()) return;

    int pista_2 = pista_1;
    for(int i = 0; i < 100; i++){
        pista_2 = dist_pista(gen);

        if(pista_2 != pista_1 && !sol.pistas[pista_2].empty()){
            break;
        }
    }

    if(pista_1 == pista_2 || sol.pistas[pista_2].empty()) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista_1].size()) - 1) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    int pos_2 = dist_pos2(gen);

    if(pos_2 < 0 || pos_2 > ((int) sol.pistas[pista_2].size()) - 1) return;

    std::swap(sol.pistas[pista_1][pos_1], sol.pistas[pista_2][pos_2]);

    atualiza_voos(sol.pistas[pista_1], pos_1);
    atualiza_voos(sol.pistas[pista_2], pos_2);

    int multa_pista_1 = sol.pistas[pista_1].back().multa_acumulada;
    int multa_pista_2 = sol.pistas[pista_2].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista_1] + sol.multa_pistas[pista_2];
    sol.multa += multa_pista_1 + multa_pista_2;
    
    sol.multa_pistas[pista_1] = multa_pista_1;
    sol.multa_pistas[pista_2] = multa_pista_2;
}

void CaixaPreta::rand_re_insertion(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista = -1;
    for(int i = 0; i < 100; i++){
        pista = dist_pista(gen);
        
        if(sol.pistas[pista].size() >= 3){
            break;
        }
    }
    
    if (pista == -1 || sol.pistas[pista].size() < 3) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista].size()) - 1) return;

    int pos_2 = pos_1;
    for(int i = 0; i < 100; i++){
        pos_2 = dist_pos1(gen);

        if(pos_2 != pos_1 && (pos_2 < pos_1 - 1 || pos_2 > pos_1 + 1)){
            break;
        }
    }

    if(pos_2 == pos_1 || (pos_2 >= pos_1 - 1 && pos_2 <= pos_1 + 1)) return;

    Voo voo_movido = sol.pistas[pista][pos_1];
    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido);

    if(pos_1 < pos_2){
        atualiza_voos(sol.pistas[pista], pos_1);
    } else{
        atualiza_voos(sol.pistas[pista], pos_2);
    }

    int multa_pista = sol.pistas[pista].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista];
    sol.multa += multa_pista;

    sol.multa_pistas[pista] = multa_pista;
}

void CaixaPreta::rand_re_insertion_pistas(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista_1 = -1;

    for(int i = 0; i < 100; i++){
        pista_1 = dist_pista(gen);

        if(!sol.pistas[pista_1].empty()){
            break;
        }
    }

    if(pista_1 == -1 || sol.pistas[pista_1].empty()) return;

    int pista_2 = pista_1;

    for(int i = 0; i < 100; i++){
        pista_2 = dist_pista(gen);

        if(pista_2 != pista_1){
            break;
        }
    }
    
    if (pista_1 == pista_2) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 1);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista_1].size()) - 1) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    int pos_2 = dist_pos2(gen);
    
    if(pos_2 < 0 || pos_2 > ((int) sol.pistas[pista_2].size()) - 1) return;
    
    Voo voo_movido = sol.pistas[pista_1][pos_1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido);

    atualiza_voos(sol.pistas[pista_1], pos_1);
    atualiza_voos(sol.pistas[pista_2], pos_2);

    int multa_pista_1 = (sol.pistas[pista_1].empty()) ? 0 : sol.pistas[pista_1].back().multa_acumulada;
    int multa_pista_2 = (sol.pistas[pista_2].empty()) ? 0 : sol.pistas[pista_2].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista_1] + sol.multa_pistas[pista_2];
    sol.multa += multa_pista_1 + multa_pista_2;
    
    sol.multa_pistas[pista_1] = multa_pista_1;
    sol.multa_pistas[pista_2] = multa_pista_2;
}

void CaixaPreta::rand_re_insertion_2(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista = -1;
    for(int i = 0; i < 100; i++){
        pista = dist_pista(gen);

        if(sol.pistas[pista].size() >= 4){
            break;
        }
    }
    
    if (pista == -1 || sol.pistas[pista].size() < 4) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 2);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista].size()) - 2) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_2 = pos_1;
    for(int i = 0; i < 100; i++){
        pos_2 = dist_pos2(gen);

        if(pos_2 != pos_1 && (pos_2 < pos_1 - 1 || pos_2 > pos_1 + 2)){
            break;
        }
    }

    if(pos_2 == pos_1 || (pos_2 >= pos_1 - 1 && pos_2 <= pos_1 + 2)) return;

    Voo voo_movido_1 = sol.pistas[pista][pos_1];
    Voo voo_movido_2 = sol.pistas[pista][pos_1+1];

    if (pos_1 < pos_2) {
        pos_2 -= 1;
    }

    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 2);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);

    if(pos_1 < pos_2){
        atualiza_voos(sol.pistas[pista], pos_1);
    } else{
        atualiza_voos(sol.pistas[pista], pos_2);
    }

    int multa_pista = sol.pistas[pista].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista];
    sol.multa += multa_pista;

    sol.multa_pistas[pista] = multa_pista;
}

void CaixaPreta::rand_re_insertion_2_pistas(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista_1 = -1;
    for(int i = 0; i < 100; i++){
        pista_1 = dist_pista(gen);

        if(sol.pistas[pista_1].size() >= 2){
            break;
        }
    }

    if(pista_1 == -1 || sol.pistas[pista_1].size() < 2) return;
    
    int pista_2 = pista_1;
    for(int i = 0; i < 100; i++){
        pista_2 = dist_pista(gen);

        if(pista_1 != pista_2){
            break;
        }
    }
    
    if (pista_1 == pista_2) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 2);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista_1].size()) - 2) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    int pos_2 = dist_pos2(gen);

    if(pos_2 < 0 || pos_2 > ((int) sol.pistas[pista_2].size()) - 1) return;

    Voo voo_movido_1 = sol.pistas[pista_1][pos_1];
    Voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 2);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);

    atualiza_voos(sol.pistas[pista_1], pos_1);
    atualiza_voos(sol.pistas[pista_2], pos_2);

    int multa_pista_1 = (sol.pistas[pista_1].empty()) ? 0 : sol.pistas[pista_1].back().multa_acumulada;
    int multa_pista_2 = sol.pistas[pista_2].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista_1] + sol.multa_pistas[pista_2];
    sol.multa += multa_pista_1 + multa_pista_2;
    
    sol.multa_pistas[pista_1] = multa_pista_1;
    sol.multa_pistas[pista_2] = multa_pista_2;
}

void CaixaPreta::rand_re_insertion_3(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista = -1;
    for(int i = 0; i < 100; i++){
        pista = dist_pista(gen);

        if(sol.pistas[pista].size() >= 5){
            break;
        }
    }
    
    if (pista == -1 || sol.pistas[pista].size() < 5) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista].size()) - 3);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista].size()) - 3) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista].size()) - 1);
    int pos_2 = pos_1;
    for(int i = 0; i < 100; i++){
        pos_2 = dist_pos2(gen);

        if(pos_2 != pos_1 && (pos_2 < pos_1 - 1 || pos_2 > pos_1 + 3)){
            break;
        }
    }

    if(pos_2 == pos_1 || (pos_2 >= pos_1 - 1 && pos_2 <= pos_1 + 3)) return;

    Voo voo_movido_1 = sol.pistas[pista][pos_1];
    Voo voo_movido_2 = sol.pistas[pista][pos_1+1];
    Voo voo_movido_3 = sol.pistas[pista][pos_1+2];

    if (pos_1 < pos_2) {
        pos_2 -= 2;
    }

    sol.pistas[pista].erase(sol.pistas[pista].begin() + pos_1, sol.pistas[pista].begin() + pos_1 + 3);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2, voo_movido_1);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 1, voo_movido_2);
    sol.pistas[pista].insert(sol.pistas[pista].begin() + pos_2 + 2, voo_movido_3);

    if(pos_1 < pos_2){
        atualiza_voos(sol.pistas[pista], pos_1);
    } else{
        atualiza_voos(sol.pistas[pista], pos_2);
    }

    int multa_pista = sol.pistas[pista].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista];
    sol.multa += multa_pista;

    sol.multa_pistas[pista] = multa_pista;
}

void CaixaPreta::rand_re_insertion_3_pistas(Solucao &sol) {
    std::uniform_int_distribution<int> dist_pista(0, dados.num_pistas - 1);
    
    int pista_1 = -1;
    for(int i = 0; i < 100; i++){
        pista_1 = dist_pista(gen);

        if(sol.pistas[pista_1].size() >= 3){
            break;
        }
    }
    
    if (pista_1 == -1 || sol.pistas[pista_1].size() < 3) return;

    int pista_2 = pista_1;
    for(int i = 0; i < 100; i++){
        pista_2 = dist_pista(gen);

        if(pista_2 != pista_1){
            break;
        }
    }

    if(pista_1 == pista_2) return;

    std::uniform_int_distribution<int> dist_pos1(0, ((int) sol.pistas[pista_1].size()) - 3);
    int pos_1 = dist_pos1(gen);

    if(pos_1 < 0 || pos_1 > ((int) sol.pistas[pista_1].size()) - 3) return;

    std::uniform_int_distribution<int> dist_pos2(0, ((int) sol.pistas[pista_2].size()) - 1);
    int pos_2 = dist_pos2(gen);

    if(pos_2 < 0 || pos_2 > ((int) sol.pistas[pista_2].size()) - 1) return;

    Voo voo_movido_1 = sol.pistas[pista_1][pos_1];
    Voo voo_movido_2 = sol.pistas[pista_1][pos_1+1];
    Voo voo_movido_3 = sol.pistas[pista_1][pos_1+2];
    sol.pistas[pista_1].erase(sol.pistas[pista_1].begin() + pos_1, sol.pistas[pista_1].begin() + pos_1 + 3);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2, voo_movido_1);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 1, voo_movido_2);
    sol.pistas[pista_2].insert(sol.pistas[pista_2].begin() + pos_2 + 2, voo_movido_3);

    atualiza_voos(sol.pistas[pista_1], pos_1);
    atualiza_voos(sol.pistas[pista_2], pos_2);

    int multa_pista_1 = (sol.pistas[pista_1].empty()) ? 0 : sol.pistas[pista_1].back().multa_acumulada;
    int multa_pista_2 = (sol.pistas[pista_2].empty()) ? 0 : sol.pistas[pista_2].back().multa_acumulada;

    sol.multa -= sol.multa_pistas[pista_1] + sol.multa_pistas[pista_2];
    sol.multa += multa_pista_1 + multa_pista_2;
    
    sol.multa_pistas[pista_1] = multa_pista_1;
    sol.multa_pistas[pista_2] = multa_pista_2;
}

void CaixaPreta::sa(Solucao &otimo) {
    long double temperatura_inicial = calcula_temperatura_inicial(otimo);
    long double temperatura_final = temperatura_inicial / 100;
    long double temperatura = temperatura_inicial;
    long double coeficiente = 0.995;
    int max_iter = dados.num_voos * 250;
    Solucao s_atual = otimo;

    std::cout << "T0: " << temperatura_inicial << '\n';
    
    while(temperatura > temperatura_final){
        for(int l = 0; l < max_iter; l++){
            Solucao s_ = generate_neighbor(s_atual);
            int variacao = s_.multa - s_atual.multa;

            if(variacao < 0){
                s_atual = s_; // Copia
                std::cout << "- " << s_atual.multa << '\n';

                if(s_atual.multa < otimo.multa){
                    otimo = s_atual; // Copia
                    l = 0;
                }
            } else{
                if(variacao == 0){
                    l--;
                    continue;
                }

                std::uniform_real_distribution<> dist(0, 1);
                double r = dist(gen);
                long double taxa = (((long double) variacao) / temperatura) * (-1);
                if(r < exp(taxa)){
                    s_atual = s_; // Copia
                    std::cout << "+ " << s_atual.multa << '\n';
                }
            }
        }

        temperatura *= coeficiente;

        std::cout << "T: " << temperatura << '\n';
    }
}

long double CaixaPreta::calcula_temperatura_inicial(Solucao &sol){
    double sum_variacao = 0;

    for(int i = 0; i < 1000; i++){
        Solucao s = generate_neighbor(sol);

        sum_variacao += (double) abs(s.multa - sol.multa) / sol.multa;
    }

    double media = (double) sum_variacao / 1000;

    return (media < 15) ? 90 : media;
}

Solucao CaixaPreta::generate_neighbor(Solucao sol){
    std::uniform_int_distribution<int> dist(1, 8);

    int k = dist(gen);

    switch(k) {
        case 1:
            rand_swap(sol);
            break;
        case 2:
            rand_re_insertion(sol);
            break;
        case 3:
            rand_swap_pistas(sol);
            break;
        case 4:
            rand_re_insertion_pistas(sol);
            break;
        case 5:
            rand_re_insertion_2(sol);
            break;
        case 6:
            rand_re_insertion_2_pistas(sol);
            break;
        case 7:
            rand_re_insertion_3(sol);
            break;
        case 8:
            rand_re_insertion_3_pistas(sol);
            break;
    }

    return sol;
}