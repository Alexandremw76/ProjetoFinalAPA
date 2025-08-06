#include <stdlib.h>
#ifdef _WIN32
#include "..\include\ILS.h"
#else
#include "../include/ILS.h"
#endif
void perturbacao(solucao *sol, Problema *p, int numTrocas)
{ // baseado no movimento de vizinhanca Swap
    for (int k = 0; k < numTrocas; k++)
    {
        // Escolha aleatória de dois servidores distintos

        int s1 = rand() % p->NServidores; // gera um numero aleatorio no intervalo de p->NServidores (numero de servidores )
        int s2 = rand() % p->NServidores;

        while (s2 == s1) // enquanto os servidores forem iguais gera outro
            s2 = rand() % p->NServidores;

        // Escolha aleatória de dois jobs 
        int i = rand() % sol->jobsPorServidor[s1]; // gera um numero aleatorio no intervalo dos numeros de jobs alocados naquele servidor
        int j = rand() % sol->jobsPorServidor[s2];

        int job1 = sol->alocacao[s1][i];
        int job2 = sol->alocacao[s2][j];

        // Simula troca de jobs entre os servidores e Calcula o tempo comsumido após a troca
        int ConsumoCapacidadeS1 = (p->CapServidores[s1] - sol->tempo_restante_Servidor[s1]) - p->tempo[s1][job1] + p->tempo[s1][job2];
        int ConsumoCapacidadeS2 = (p->CapServidores[s2] - sol->tempo_restante_Servidor[s2]) - p->tempo[s2][job2] + p->tempo[s2][job1];

        // Verifica se a troca respeita a capacidade do servidor
        if (ConsumoCapacidadeS1 <= p->CapServidores[s1] && ConsumoCapacidadeS2 <= p->CapServidores[s2])
        {
            // Troca dos jobs entre os servidores selecionados
            sol->alocacao[s1][i] = job2;
            sol->alocacao[s2][j] = job1;

            // Atualização dos custos na solução
            sol->custoServidor[s1] = (sol->custoServidor[s1] - p->custo[s1][job1]) + p->custo[s1][job2];
            sol->custoServidor[s2] = (sol->custoServidor[s2] - p->custo[s2][job2]) + p->custo[s2][job1];

            // Atualização do tempo restante nos servidores afetados
            sol->tempo_restante_Servidor[s1] = p->CapServidores[s1] - ConsumoCapacidadeS1;
            sol->tempo_restante_Servidor[s2] = p->CapServidores[s2] - ConsumoCapacidadeS2;
        }
    }
    int cont = 0;
    for (int s1 = 0; s1 < p->NServidores; s1++)
        cont += sol->custoServidor[s1];

    sol->custoTotal = cont + sol->custoLocal;
}

solucao *ILS(solucao *sol, Problema *p, int maxIteracoes, int numTrocasPerturbacao)
{
    int iteracao = 0;
    solucao *melhorSolucao = copiarSolucao(sol, p); // Melhor solução encontrada até o momento (soluçao q ja passou pelo VND)

    while (iteracao < maxIteracoes)
    {

        // Perturba a solução atual
        perturbacao(sol, p, numTrocasPerturbacao);

        // Realiza busca local na solução perturbada.
        VND(sol, p);

        // Atualiza a melhor solução encontrada, se necessário
        if (sol->custoTotal < melhorSolucao->custoTotal)
        {
            liberarSolucao(p, melhorSolucao); // Liberar memória da melhor solução anterior
            melhorSolucao = copiarSolucao(sol, p);
        }

        iteracao++;
    }

    return melhorSolucao;
}