#ifndef GULOSO_H
#define GULOSO_H
#include <limits.h>

#ifdef _WIN32
#include "..\include\estruturas.h"
#else
#include "../include/estruturas.h"
#endif

void algoritmoGuloso(solucao *sol , Problema *p);

#endif