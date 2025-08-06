#ifdef _WIN32
#include "..\include\func_aux.h"
#else
#include "../include/func_aux.h"
#endif

Problema *lerDados(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    Problema *p = (Problema *)malloc(sizeof(Problema));
    fscanf(arquivo, "%d %d %d", &p->Njobs, &p->NServidores, &p->CLocal);

    // Aloca memória para b, t, c
    p->CapServidores = (int *)malloc(p->NServidores * sizeof(int));
    p->tempo = (int **)malloc(p->NServidores * sizeof(int *));
    p->custo = (int **)malloc(p->NServidores * sizeof(int *));
    for (int s = 0; s < p->NServidores; s++)
    {
        p->tempo[s] = (int *)malloc(p->Njobs * sizeof(int));
        p->custo[s] = (int *)malloc(p->Njobs * sizeof(int));
    }

    // Lê capacidades dos servidores
    for (int s = 0; s < p->NServidores; s++)
    {
        fscanf(arquivo, "%d", &p->CapServidores[s]);
    }

    // Lê matrizes t e c
    for (int s = 0; s < p->NServidores; s++)
    {
        for (int j = 0; j < p->Njobs; j++)
        {
            fscanf(arquivo, "%d", &p->tempo[s][j]);
        }
    }
    for (int s = 0; s < p->NServidores; s++)
    {
        for (int j = 0; j < p->Njobs; j++)
        {
            fscanf(arquivo, "%d", &p->custo[s][j]);
        }
    }

    fclose(arquivo);
    return p;
}

solucao *AlocarSolucao(Problema *p)
{
    solucao *sol = (solucao *)malloc(sizeof(solucao));
    sol->custoTotal = 0;
    sol->custoLocal = 0;
    sol->custoServidor = (int *)calloc(p->NServidores, sizeof(int));
    sol->jobsPorServidor = (int *)calloc(p->NServidores, sizeof(int));
    sol->JobsAlocados = (int *)calloc(p->Njobs, sizeof(int)); // Guarda a informaçao se job ta sendo executado localmente ou em um servirdor
    sol->alocacao = (int **)malloc(p->NServidores * sizeof(int *));

    for (int i = 0; i < p->NServidores; i++)                      // p->NServidores = numero de servidores
        sol->alocacao[i] = (int *)malloc(p->Njobs * sizeof(int)); // Alocar espaço para todos os jobs, mesmo que nem todos sejam usados

    sol->tempo_restante_Servidor = (int *)malloc(p->NServidores * sizeof(int));
    memcpy(sol->tempo_restante_Servidor, p->CapServidores, p->NServidores * sizeof(int)); // Copiar capacidades dos servidores

    return sol;
}

void liberarMemoria(Problema *p, solucao *sol, solucao *sol1)
{
    if (sol != NULL)
    {
        free(sol->custoServidor);
        free(sol->jobsPorServidor);
        free(sol->JobsAlocados);
        free(sol->tempo_restante_Servidor);

        for (int i = 0; i < p->NServidores; i++)
        {
            free(sol->alocacao[i]);
        }
        free(sol->alocacao);
        free(sol);
    }

    if (sol1 != NULL)
    {
        free(sol1->custoServidor);
        free(sol1->jobsPorServidor);
        free(sol1->JobsAlocados);
        free(sol1->tempo_restante_Servidor);

        for (int i = 0; i < p->NServidores; i++)
        {
            free(sol1->alocacao[i]);
        }
        free(sol1->alocacao);
        free(sol1);
    }

    if (p != NULL)
    {
        free(p->CapServidores);
        for (int s = 0; s < p->NServidores; s++)
        {
            free(p->tempo[s]);
            free(p->custo[s]);
        }
        free(p->tempo);
        free(p->custo);
        free(p);
    }
}

void liberarSolucao(Problema *p, solucao *sol)
{
    if (sol != NULL)
    {
        free(sol->custoServidor);
        free(sol->jobsPorServidor);
        free(sol->JobsAlocados);
        free(sol->tempo_restante_Servidor);

        for (int i = 0; i < p->NServidores; i++)
        {
            free(sol->alocacao[i]);
        }
        free(sol->alocacao);
        free(sol);
    }
}

void imprimirResultados(const Problema *p, const solucao *sol, int tipo)
{
    if (tipo == 1)
    {
        printf("\n-----------ALGORITMO GULOSO------------\n");
    }
    else if (tipo == 2)
    {
        printf("\n----------------VND---------------------\n");
    }
    else
    {
        printf("\n---------------BUSCA LOCAL ITERADA(ILS)---------------------\n");
    }
    printf("\nCusto total: %d\n", sol->custoTotal);
    printf("Custo de alocacao nos servidores: ");
    for (int i = 0; i < p->NServidores; i++)
    {
        printf("%d ", sol->custoServidor[i]);
    }
    printf("\nCusto associado a jobs executados localmente: %d\n", sol->custoLocal);

    // Imprime a lista de jobs alocados em cada servidor
    for (int s = 0; s < p->NServidores; s++)
    {
        printf("\nLista de jobs alocados no servidor %d: ", s + 1);
        for (int j = 0; j < sol->jobsPorServidor[s]; j++)
        {
            printf("%d ", sol->alocacao[s][j] + 1); // +1 para ajustar a indexação
        }
    }

    for (int s = 0; s < p->NServidores; s++)
    {
        printf("\nTempo restante no servidor %d: %d", s + 1, sol->tempo_restante_Servidor[s]);
    }
}

void EscreverResultadosArquivo(const Problema *p, const solucao *sol, const char *caminhoArquivo)
{
    FILE *arquivo = fopen(caminhoArquivo, "w"); // Abre o arquivo para escrita
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(arquivo, "%d\n", sol->custoTotal);
    int cont = 0;
    for (int i = 0; i < p->NServidores; i++)
    {
        cont += sol->custoServidor[i];
    }
    fprintf(arquivo, "%d ", cont);
    fprintf(arquivo, "\n%d\n", sol->custoLocal);

    // Escreve a lista de jobs alocados em cada servidor
    for (int s = 0; s < p->NServidores; s++)
    {
        fprintf(arquivo, "\n");
        for (int j = 0; j < sol->jobsPorServidor[s]; j++)
        {
            fprintf(arquivo, "%d ", sol->alocacao[s][j] + 1); // +1 para ajustar a indexação
        }
    }

    fclose(arquivo); // Fecha o arquivo
}
void construirCaminho(const char *prefixo, const char *nomeArquivo, char *caminho)
{ // concatenar as strings prefixo e nomeArquivo em caminho e nao podendo passar de 50 caracteres
    snprintf(caminho, 50, "%s%s", prefixo, nomeArquivo);
}

void Escrever_tempo(const char *caminhoArquivo, double *tempo, const char *instacia)
{
    FILE *arquivo = fopen(caminhoArquivo, "w"); // Abre o arquivo para escrita
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arquivo, "Tempo de processamento gasto Guloso para instancia ");
    fprintf(arquivo, "%s : ", instacia);
    fprintf(arquivo, "%lf Segundos \n", tempo[0]);
    fprintf(arquivo, "Tempo de processamento gasto VND para instancia ");
    fprintf(arquivo, "%s : ", instacia);
    fprintf(arquivo, "%lf Segundos \n", tempo[1]);
    fprintf(arquivo, "Tempo de processamento gasto ILS(BUSCA LOCAL ITERADA) para instancia ");
    fprintf(arquivo, "%s : ", instacia);
    fprintf(arquivo, "%lf Segundos \n", tempo[2]);
}
solucao *copiarSolucao(solucao *origem, Problema *p)
{

    solucao *destino = (solucao *)malloc(sizeof(solucao));
    // Aloca memória para os arrays de inteiros
    destino->jobsPorServidor = (int *)malloc(p->NServidores * sizeof(int));
    destino->custoServidor = (int *)malloc(p->NServidores * sizeof(int));
    destino->tempo_restante_Servidor = (int *)malloc(p->NServidores * sizeof(int));
    destino->JobsAlocados = (int *)malloc(p->Njobs * sizeof(int));

    // Copia os valores dos arrays de int
    memcpy(destino->jobsPorServidor, origem->jobsPorServidor, p->NServidores * sizeof(int));
    memcpy(destino->custoServidor, origem->custoServidor, p->NServidores * sizeof(int));
    memcpy(destino->tempo_restante_Servidor, origem->tempo_restante_Servidor, p->NServidores * sizeof(int));
    memcpy(destino->JobsAlocados, origem->JobsAlocados, p->Njobs * sizeof(int));

    // Aloca memória para a matriz de alocação
    destino->alocacao = (int **)malloc(p->NServidores * sizeof(int *));
    for (int i = 0; i < p->NServidores; i++)
    {
        destino->alocacao[i] = (int *)malloc(destino->jobsPorServidor[i] * sizeof(int));
        memcpy(destino->alocacao[i], origem->alocacao[i], origem->jobsPorServidor[i] * sizeof(int));
    }

    // Copia os outros campos
    destino->custoTotal = origem->custoTotal;
    destino->custoLocal = origem->custoLocal;

    return destino;
}