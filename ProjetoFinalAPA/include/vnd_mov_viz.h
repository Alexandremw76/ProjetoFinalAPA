#ifndef VND_MOV_VIZ_H
#define VND_MOV_VIZ_H

#ifdef _WIN32
#include "..\include\estruturas.h"
#else
#include "../include/estruturas.h"
#endif

#include <stdbool.h>
void VND(solucao *sol, Problema *p);
bool Swap(solucao *sol, Problema *p);
bool Swap2opt(solucao *sol, Problema *p);
bool Shift(solucao *sol, Problema *p);
#endif