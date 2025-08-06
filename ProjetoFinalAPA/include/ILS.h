#ifndef ILS_H
#define ILS_H

#ifdef _WIN32
#include "..\include\vnd_mov_viz.h"
#include "..\include\estruturas.h"
#include "..\include\func_aux.h"
#else
#include "../include/vnd_mov_viz.h"
#include "../include/estruturas.h"
#include "../include/func_aux.h"
#endif
void perturbacao(solucao *sol, Problema *p, int numTrocas);
solucao *ILS(solucao *sol, Problema *p, int maxIteracoes, int numTrocasPerturbacao);
#endif