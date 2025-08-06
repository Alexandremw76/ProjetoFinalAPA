#ifdef _WIN32
#include "..\include\vnd_mov_viz.h"
#else
#include "../include/vnd_mov_viz.h"
#endif

bool Swap(solucao *sol, Problema *p)
{ // (Swap) Troca de Jobs entre Servidores
    bool melhorou = false;

    for (int s1 = 0; s1 < p->NServidores; s1++)
    {
        for (int s2 = 0; s2 < p->NServidores; s2++)
        {
            if (s1 == s2)
                continue; // Não faz sentido trocar jobs no mesmo servidor (trocar a ordem dos jobs em um mesmo sevidor nao reduz o custo)
            for (int i = 0; i < sol->jobsPorServidor[s1]; i++)
            {
                for (int j = 0; j < sol->jobsPorServidor[s2]; j++)
                {

                    int job1 = sol->alocacao[s1][i];
                    int job2 = sol->alocacao[s2][j];

                    // Simula troca de jobs entre os servidores e Calcula o tempo comsumido após a troca
                    int ConsumoCapacidadeS1 = (p->CapServidores[s1] - sol->tempo_restante_Servidor[s1] - p->tempo[s1][job1] + p->tempo[s1][job2]);
                    int ConsumoCapacidadeS2 = (p->CapServidores[s2] - sol->tempo_restante_Servidor[s2] - p->tempo[s2][job2] + p->tempo[s2][job1]);

                    // Verificar se a troca é possível (capacidade dos servidores não é violada) tempo consumido é menor ou igual q o tempo disponivel no servidor
                    if (ConsumoCapacidadeS1 <= p->CapServidores[s1] && ConsumoCapacidadeS2 <= p->CapServidores[s2])
                    {

                        // Simula troca de jobs entre os servidores e Calcula o custo após a troca
                        int custoDepoisS1 = (sol->custoServidor[s1] - p->custo[s1][job1] + p->custo[s1][job2]);
                        int custoDepoisS2 = (sol->custoServidor[s2] - p->custo[s2][job2] + p->custo[s2][job1]);

                        // calcula o custo geral ante e depois de simular a troca
                        int custoTotalAntes = sol->custoServidor[s1] + sol->custoServidor[s2];
                        int custoTotalDepois = custoDepoisS1 + custoDepoisS2;

                        // Se houver melhoria, realiza a troca
                        if (custoTotalDepois < custoTotalAntes)
                        {
                            sol->alocacao[s1][i] = job2;
                            sol->alocacao[s2][j] = job1;

                            // Atualiza os custos na solucao
                            sol->custoServidor[s1] = custoDepoisS1;
                            sol->custoServidor[s2] = custoDepoisS2;
                            sol->tempo_restante_Servidor[s1] = p->CapServidores[s1] - ConsumoCapacidadeS1;
                            sol->tempo_restante_Servidor[s2] = p->CapServidores[s2] - ConsumoCapacidadeS2;
                            melhorou = true;
                        }
                    }
                }
            }
        }
    }
    // atualiar custo da soluçao
    sol->custoTotal = 0;
    for (int s1 = 0; s1 < p->NServidores; s1++)
        sol->custoTotal += sol->custoServidor[s1];

    sol->custoTotal += sol->custoLocal;

    return melhorou;
}

bool Swap2opt(solucao *sol, Problema *p)
{
    bool melhorou = false;

    for (int s1 = 0; s1 < p->NServidores; s1++)
    {
        for (int s2 = 0; s2 < p->NServidores; s2++)
        {
            if (s1 == s2)
                continue; // Ignora trocas no mesmo servidor.

            // Varre todos os pares de jobs em servidores diferentes.
            for (int i = 0; i < sol->jobsPorServidor[s1]; i++)
            {
                for (int j = 0; j < sol->jobsPorServidor[s2]; j++)
                {
                    for (int i2 = i + 1; i2 < sol->jobsPorServidor[s1]; i2++) // Segundo job no primeiro servidor.
                    {
                        for (int j2 = j + 1; j2 < sol->jobsPorServidor[s2]; j2++) // Segundo job no segundo servidor.
                        {
                            int job1 = sol->alocacao[s1][i];
                            int job2 = sol->alocacao[s2][j];
                            int job3 = sol->alocacao[s1][i2];
                            int job4 = sol->alocacao[s2][j2];

                            // Simula a troca de dois jobs entre os servidores e calcula o consumo de capacidade e custo.
                            int novoConsumoS1 = (p->CapServidores[s1] - sol->tempo_restante_Servidor[s1]) + p->tempo[s1][job2] + p->tempo[s1][job4] - p->tempo[s1][job1] - p->tempo[s1][job3]; // Simular Trocar 2 e 4 pelo 1 e 3
                            int novoConsumoS2 = (p->CapServidores[s2] - sol->tempo_restante_Servidor[s2]) + p->tempo[s2][job1] + p->tempo[s2][job3] - p->tempo[s2][job2] - p->tempo[s2][job4]; // Simular Trocar 1 e 3 pelo 2 e 4

                            if (novoConsumoS1 <= p->CapServidores[s1] && novoConsumoS2 <= p->CapServidores[s2])
                            {
                                int novoCustoS1 = (sol->custoServidor[s1] - p->custo[s1][job1] - p->custo[s1][job3] + p->custo[s1][job2] + p->custo[s1][job4]);
                                int novoCustoS2 = (sol->custoServidor[s2] - p->custo[s2][job2] - p->custo[s2][job4] + p->custo[s2][job1] + p->custo[s2][job3]);

                                int custoTotalAntes = sol->custoServidor[s1] + sol->custoServidor[s2];
                                int custoTotalDepois = novoCustoS1 + novoCustoS2;

                                if (custoTotalDepois < custoTotalAntes) // Se a troca reduz o custo total, realiza a troca.
                                {
                                    sol->alocacao[s1][i] = job2;
                                    sol->alocacao[s2][j] = job1;
                                    sol->alocacao[s1][i2] = job4;
                                    sol->alocacao[s2][j2] = job3;

                                    sol->custoServidor[s1] = novoCustoS1;
                                    sol->custoServidor[s2] = novoCustoS2;
                                    sol->tempo_restante_Servidor[s1] = p->CapServidores[s1] - novoConsumoS1;
                                    sol->tempo_restante_Servidor[s2] = p->CapServidores[s2] - novoConsumoS2;
                                    melhorou = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // atualiar custo da soluçao
    sol->custoTotal = 0;
    for (int s1 = 0; s1 < p->NServidores; s1++)
        sol->custoTotal += sol->custoServidor[s1];

    sol->custoTotal += sol->custoLocal;

    return melhorou;
}

bool Shift(solucao *sol, Problema *p)
{ // Shift (Deslocamento de um Job para Outro Servidor)
    bool melhorou = false;

    for (int s1 = 0; s1 < p->NServidores; s1++)
    {
        for (int s2 = 0; s2 < p->NServidores; s2++)
        {
            if (s1 == s2)
                continue; // Não faz sentido trocar jobs no mesmo servidor
            for (int i = 0; i < sol->jobsPorServidor[s1]; i++)
            {

                int job1 = sol->alocacao[s1][i];

                // Simula a realocaçao de um job para outro servidor e Calcula o tempo comsumido após a realocaçao
                int ConsumoCapacidadeS1 = (p->CapServidores[s1] - sol->tempo_restante_Servidor[s1]) - p->tempo[s1][job1];
                int ConsumoCapacidadeS2 = (p->CapServidores[s2] - sol->tempo_restante_Servidor[s2]) + p->tempo[s2][job1];

                if (ConsumoCapacidadeS2 <= p->CapServidores[s2])
                { // Verificar se a realocaçao é possível (capacidade do servidor não é violada) tempo consumido é menor ou igual q o tempo disponivel no servidor
                    // Calcula o custo depois da troca
                    int custoDepoisS1 = (sol->custoServidor[s1] - p->custo[s1][job1]);
                    int custoDepoisS2 = (sol->custoServidor[s2] + p->custo[s2][job1]);

                    // Calcula o custo geral antes da realocaçao
                    int custoTotalAntes = sol->custoServidor[s1] + sol->custoServidor[s2];

                    int custoTotalDepois = custoDepoisS1 + custoDepoisS2; // custo geral depois
                    // Se houver melhoria geral, realiza a troca
                    if (custoTotalDepois < custoTotalAntes)
                    {
                        sol->alocacao[s2][sol->jobsPorServidor[s2]] = job1; // adiciona job a matriz de aloçao de jobs
                        sol->jobsPorServidor[s2]++;                         // aumenta o numero de jobs no servidor

                        for (int r = i; r < sol->jobsPorServidor[s1] - 1; r++)
                        { // remove o job da matriz de alocaçao de jobs
                            sol->alocacao[s1][r] = sol->alocacao[s1][r + 1];
                        }
                        sol->jobsPorServidor[s1]--; // diminui o numero de jobs no servidor

                        // Atualiza os custos na solucao
                        sol->custoServidor[s1] = custoDepoisS1;
                        sol->custoServidor[s2] = custoDepoisS2;
                        sol->tempo_restante_Servidor[s1] = p->CapServidores[s1] - ConsumoCapacidadeS1;
                        sol->tempo_restante_Servidor[s2] = p->CapServidores[s2] - ConsumoCapacidadeS2;
                        melhorou = true;
                    }
                }
            }
        }
    }
    int cont = 0;
    for (int s1 = 0; s1 < p->NServidores; s1++)
        cont += sol->custoServidor[s1];

    sol->custoTotal = cont + sol->custoLocal;

    return melhorou;
}

void VND(solucao *sol, Problema *p)
{
    // Vnd com o objetivo de otimizar o custo gasto em cada servidor.
    int melhoria = 1;
    while (melhoria)
    {
        melhoria = 0;
        if (Shift(sol, p))
        {
            melhoria = 1;
            continue;
        }
        if (Swap2opt(sol, p))
        {
            melhoria = 1;
            continue;
        }
        if (Swap(sol, p))
        {
            melhoria = 1;
            continue;
        }
    }
}