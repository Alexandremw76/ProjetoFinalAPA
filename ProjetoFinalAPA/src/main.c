#ifdef _WIN32
#include "..\include\estruturas.h"
#include "..\include\guloso.h"
#include "..\include\vnd_mov_viz.h"
#include "..\include\ILS.h"
#include "..\include\func_aux.h"
#else
#include "../include/estruturas.h"
#include "../include/guloso.h"
#include "../include/vnd_mov_viz.h"
#include "../include/ILS.h"
#include "../include/func_aux.h"
#endif
#include <time.h>
/*
n5m15A = 261
n5m15B = 269
n25m5A = 438
n30m5A = 423
n60m10 = 954
n60m10A = 973
*/



const char *nomesArquivos_entrada[] = {"n5m15A.txt", "n5m15B.txt", "n25m5A.txt", "n30m5A.txt", "n60m10.txt", "n60m10A.txt"};

#ifdef _WIN32
const char prefixo_entrada[] = {"in\\"};
const char prefixo_tempo_gasto[] = {"Tempo_gasto\\"};
const char prefixo_saida_guloso[] = {"out\\guloso\\"};
const char prefixo_saida_vnd[] = {"out\\vnd\\"};
const char prefixo_saida_ils[] = {"out\\ils\\"};
#else
const char prefixo_entrada[] = {"in/"};
const char prefixo_tempo_gasto[] = {"Tempo_gasto/"};
const char prefixo_saida_guloso[] = {"out/guloso/"};
const char prefixo_saida_vnd[] = {"out/vnd/"};
const char prefixo_saida_ils[] = {"out/ils/"};

#endif

int main()
{
    // Loop sobre os arquivos de entrada
    for (int i = 0; i < sizeof(nomesArquivos_entrada) / sizeof(nomesArquivos_entrada[0]); i++)
    {
        char caminho_entrada[50];
        char caminho_saida_guloso[50];
        char caminho_saida_vnd[50];
        char caminho_saida_ils[50];
        char tempo_gasto_saida[50];
        struct timespec comeco, fim;

        double tempo_gasto[3]; // array para armazenar o tempo gasto em cada algoritmo
        // Construir caminhos de entrada e saída
        construirCaminho(prefixo_tempo_gasto, nomesArquivos_entrada[i], tempo_gasto_saida);
        construirCaminho(prefixo_entrada, nomesArquivos_entrada[i], caminho_entrada);
        construirCaminho(prefixo_saida_guloso, nomesArquivos_entrada[i], caminho_saida_guloso);
        construirCaminho(prefixo_saida_vnd, nomesArquivos_entrada[i], caminho_saida_vnd);
        construirCaminho(prefixo_saida_ils, nomesArquivos_entrada[i], caminho_saida_ils);

        // Ler dados do arquivo de entrada
        Problema *p = lerDados(caminho_entrada);
        if (p == NULL)
        {
            printf("Erro ao ler dados do arquivo: %s\n", caminho_entrada);
            continue; // Avançar para o próximo arquivo
        }

        // Executar algoritmo guloso
        solucao *sol = AlocarSolucao(p);
        timespec_get(&comeco, TIME_UTC);
        algoritmoGuloso(sol,p); // Executar o algoritmo guloso (construtivo)
        timespec_get(&fim, TIME_UTC);
        tempo_gasto[0] = difftime(fim.tv_sec, comeco.tv_sec) + (fim.tv_nsec - comeco.tv_nsec) / 1000000000.0;// salvar tempo gasto

        // Imprimir e escrever resultados do algoritmo guloso
        printf("\n\n\n------------Resultado para a instancia : %s (Algoritmo Guloso)--------------", nomesArquivos_entrada[i]);
        imprimirResultados(p, sol, 1); // tipo da solução gulosa
        EscreverResultadosArquivo(p, sol, caminho_saida_guloso);
        printf("\nTempo gasto Guloso: %.6lf segundos", tempo_gasto[1]);

        // Executar VND
        timespec_get(&comeco, TIME_UTC);
        VND(sol, p);
        timespec_get(&fim, TIME_UTC);
        tempo_gasto[1] = difftime(fim.tv_sec, comeco.tv_sec) + (fim.tv_nsec - comeco.tv_nsec) / 1000000000.0;// salvar tempo gasto

        // Imprimir e escrever resultados do VND
        printf("\n\n\n------------Resultado para a instancia : %s (VND)--------------", nomesArquivos_entrada[i]);
        imprimirResultados(p, sol, 2); // tipo da solução VND
        EscreverResultadosArquivo(p, sol, caminho_saida_vnd);
        printf("\nTempo gasto VND: %.6lf segundos", tempo_gasto[1]);

        // Executar ILS
        timespec_get(&comeco, TIME_UTC);
        solucao *melhorSolucao = ILS(sol, p, 10000, 10); // 50.000 iteraçoes fazendo 10 trocas aleatorias entre os servidores
        timespec_get(&fim, TIME_UTC);
        tempo_gasto[2] = difftime(fim.tv_sec, comeco.tv_sec) + (fim.tv_nsec - comeco.tv_nsec) / 1000000000.0; // salvar tempo gasto
        
        // Imprimir e escrever resultados do ILS
        printf("\n\n\n------------Resultado para a instancia : %s (ILS)--------------", nomesArquivos_entrada[i]);
        imprimirResultados(p, melhorSolucao, 3); // tipo da solução ILS
        EscreverResultadosArquivo(p, melhorSolucao, caminho_saida_ils);
        Escrever_tempo(tempo_gasto_saida, tempo_gasto, nomesArquivos_entrada[i]);
        printf("\nTempo gasto ILS: %.6lf segundos", tempo_gasto[2]);

        liberarMemoria(p, sol, melhorSolucao); // liberar memória alocada para o problema e soluções
    }
    return 0;

}
