#include "include/algoritmos.h"

/*
    Este algoritmo possui uma estrategia FIFO (First in First Out)
    Onde o voo com o menor horario de decolagem irá ser escolhido para a pista.

    Variavéis:

    Voos = array de voos por ordem de decolagem
    Matrix = uma tabela de dados representando os tempos de espera entre voos
    Num_voos = numero total de voos a partir
    Num_pistas = numero de pistas disponíveis 
*/

void algoritmo_1(const voo* voos, int** matrix, int num_voos, int num_pistas){
    
    int t_pistas[num_pistas]; // Representa o tempo da pista
    int fila = 0; // Indice do próximo a ir para a pista
    int multa = 0; // Valor da multa

    std::vector<int> pistas[num_pistas]; // Output

    for (int i = 0; i < num_pistas; i++){
        t_pistas[i] = 0;
    }

    int c = 0;

    // Ciclo de execução
    while(true){
        for(int i = 0; i < num_pistas && fila < num_voos; i++){ 
            if(t_pistas[i] > 0){ // Se tiver voo decolando, atualiza o tempo da pista
                t_pistas[i] -= 1;
            }

            if(t_pistas[i] == 0){ // Se tiver pista vazia, faz o procedimento de inserção de voo
                int t_espera = 0;
                int t_vazio = 0;
                
                if(!pistas[i].empty()){ // Se um voo já decolou antes, procura o tempo de espera entre o voo anterior e esse de agora
                    t_espera = matrix[pistas[i].back() - 1][voos[fila].id - 1];
                }
                
                if(c + t_espera < voos[fila].t_decolagem){ // Se o voo de agora vai demorar para decolar, acrescenta o tempo de pista vazia
                    t_vazio = voos[fila].t_decolagem - (c + t_espera);
                }
                else if (c + t_espera > voos[fila].t_decolagem){ // Se o voo de agora atrasou, acrescenta o valor da multa
                    multa += voos[fila].multa * (c + t_espera - voos[fila].t_decolagem);
                }

                // Atualiza o tempo da pista e insira voo no output
                t_pistas[i] += voos[fila].duracao + t_espera + t_vazio; 
                pistas[i].push_back(voos[fila].id);
                fila++;
            }
        }

        if(fila == num_voos){
            break;
        }

        c++;
    }

    // Exibe resultado
    std::cout << multa << std::endl;
    
    for(int i = 0; i < num_pistas; i++){
        for(int id : pistas[i]){
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }

}