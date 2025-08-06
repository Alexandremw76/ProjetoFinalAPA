#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct
{
    int Njobs;          // Número de jobs
    int NServidores;    // Número de servidores
    int CLocal;         // Custo de execução local
    int *CapServidores; // Capacidade dos servidores
    int **tempo;        // Matriz de tempos de execução: t[servidor][job]
    int **custo;        // Matriz de custos: c[servidor][job]
} Problema;

typedef struct
{
    int *jobsPorServidor;         // Armazena o número de jobs alocados por servidor
    int **alocacao;               // Matriz de alocacao: alocacao[servidor][índice] = job
    int custoTotal;               // custo total da soluçao
    int *custoServidor;           // Custo total por servidor
    int *tempo_restante_Servidor; // Tempo restante no servidor
    int *JobsAlocados;            // Jobs alocados
    int custoLocal;               // Custo total de execução local
} solucao;
#endif
