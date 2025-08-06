#ifdef _WIN32
#include "..\include\guloso.h"
#else
#include "../include/guloso.h"
#endif

void algoritmoGuloso(solucao *sol , Problema *p)
{
    for (int j = 0; j < p->Njobs; j++)
    {                               // p->Njobs = numero de jobs
        int menortempo = INT_MAX;   // Menor tempo de execução comeca com o maior valor possivel
        int servidorEscolhido = -1; // -1 como nenhum servidor escolhido
        for (int s = 0; s < p->NServidores; s++)
        { 
            if (sol->tempo_restante_Servidor[s] >= p->tempo[s][j] && p->tempo[s][j] < menortempo) // se houver tempo restante no servidor e o tempo de execução do job no servidor for menor que o menortempo
            { 
                menortempo = p->tempo[s][j];
                servidorEscolhido = s;
            }
        }
        if (servidorEscolhido != -1)
        { // Se encontrou um servidor
            // Alocar no servidor
            sol->tempo_restante_Servidor[servidorEscolhido] -= p->tempo[servidorEscolhido][j]; // Diminuir o tempo restante do servidor escolhido pelo tempo de execução do job
            sol->custoServidor[servidorEscolhido] += p->custo[servidorEscolhido][j]; // Adicionar o custo do job ao custo do servidor
            sol->alocacao[servidorEscolhido][sol->jobsPorServidor[servidorEscolhido]] = j; // Adicionar o job ao servidor
            sol->jobsPorServidor[servidorEscolhido]++; // Incrementar o numero de jobs no servidor
            sol->JobsAlocados[j] = 1; // Marcar o job como alocado
            sol->custoTotal += p->custo[servidorEscolhido][j]; // Adicionar o custo do job ao custo total
        }
        else
        { // se nao encontrou um servidor
            // Executar localmente
            sol->custoLocal += p->CLocal;
            sol->custoTotal += p->CLocal;
        }
    }
}
