#ifndef FUNC_AUX_H
#define FUNC_AUX_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include "..\include\estruturas.h"
#else
#include "../include/estruturas.h"
#endif

Problema *lerDados(const char *nomeArquivo);
solucao *copiarSolucao(solucao *origem, Problema *p);
solucao *AlocarSolucao(Problema *p);
void liberarMemoria(Problema *p, solucao *sol, solucao *sol1);
void liberarSolucao(Problema *p, solucao *sol);
void imprimirResultados(const Problema *p, const solucao *sol, int tipo);
void EscreverResultadosArquivo(const Problema *p, const solucao *sol, const char *caminhoArquivo);
void construirCaminho(const char *prefixo, const char *nomeArquivo, char *caminho);
void Escrever_tempo(const char *caminhoArquivo, double *tempo, const char *instacia);


#endif