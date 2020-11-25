// eclus.cpp

#include <vector>
#include <set>

#include "eclus.h"
#include "cdb.h"
#include "graph.h"

using namespace std; 

// i > j
long etovar(unsigned int i, unsigned int j) {
    unsigned long i_tmp=i; 
    return i_tmp*(i_tmp)/2 + j + 1;
}

clause triag(long v1, long v2, long v3) {
    clause cl_r; 
    cl_r.push_back(-v1); 
    cl_r.push_back(-v2); 
    cl_r.push_back(v3); 
    return cl_r; 
}

void eclus(graph &corr, clauses &cls) {
    clause cl_t; 
    // set<edgew>::iterator sit; 

    long var1, var2, var3; 

    cls.set_nvar(corr.get_nver() * (corr.get_nver()-1)/2); 

    // hard clauses
    for (unsigned int i = 0; i < corr.get_nver(); i++) {
        for (unsigned int j = 0; j < i; j++) {
            var1 = etovar(i,j);
            for (unsigned int k = 0; k < j; k++) {
                var2 = etovar(i,k);
                var3 = etovar(j,k); 

                // add triangle
                cls.add(triag(var1, var2, var3)); 
                cls.add(triag(var1, var3, var2)); 
                cls.add(triag(var2, var3, var1)); 
            }
        }
    }
    
    // soft clauses
    for (unsigned int i = 0; i < corr.get_nver(); i++) {
        unsigned int w;
        for (unsigned int j = 0; j < i; j++) { 
            // j < i, pos w
            w = corr.get_weight(i,j); 
            if(w) {
                cl_t = clause({etovar(i,j)}); 
                cls.add(cl_t, w); 
            }
        }
        for (unsigned int j = i+1; j < corr.get_nver(); j++) { 
            // j > i, neg w
            w = corr.get_weight(i,j); 
            if(w) {
                cl_t = clause({-etovar(j,i)}); 
                cls.add(cl_t, w);
            }
        }
    }
}
