#ifndef ECLUS_H
#define ECLUS_H

#include "graph.h"
#include "cdb.h"

void eclus(graph &corr, clauses &cls); 
void eclus(graph &corr, clauses &cls, unsigned int plen); 

#endif
