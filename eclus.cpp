// eclus.cpp

#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <iostream>
#include <stack>

#include "eclus.h"
#include "cdb.h"
#include "graph.h"

using namespace std; 

// i > j
long etovar(unsigned int i, unsigned int j) {
    unsigned long i_tmp=i; 
    return i_tmp*(i_tmp-1)/2 + j + 1;
}

clause triag(long v1, long v2, long v3) {
    clause cl_r; 
    cl_r.push_back(-v1); 
    cl_r.push_back(-v2); 
    cl_r.push_back(v3); 
    return cl_r; 
}

void ppath(clauses &cls, vector<unsigned int> &vertices) {
    clause cl_ref; // all neg literals 
    clause cl_t; 
    unsigned int n_ver = vertices.size(); 
    unsigned int sml, big; 
    for (unsigned int i=0; i < n_ver-1; i++) {
        sml = min(vertices[i], vertices[i+1]); 
        big = max(vertices[i], vertices[i+1]); 
        cl_ref.push_back(-etovar(big,sml)); 
    }
    sml = min(vertices[n_ver-1], vertices[0]); 
    big = max(vertices[n_ver-1], vertices[0]); 
    cl_ref.push_back(-etovar(big,sml)); 

    // flip one literal
    for (unsigned int i=0; i < n_ver; i++) {
        cl_t = clause(cl_ref); 
        cl_t[i] = -cl_ref[i]; 
        cls.add(cl_t); 
    }
}

void disall(vector<vector<unsigned int>> &paths, unsigned int plen) {
    unsigned int j; 
    vector<unsigned int> vt; 
    vector<bool> mask_per, mask; 
    vector<vector<unsigned int>> clq; 
    for (j=0; j<=plen; j++) {
        vt.push_back(j); 
        mask_per.push_back(true); 
    }
    for (j=0; j<=plen; j++) {
        clq.push_back(vt); 
        clq[j].erase(clq[j].begin()+j); 
    }

    // cout << clq[plen][plen-2] << endl; 

    unsigned int v_curr;

    stack<unsigned int> dstack; 
    stack<unsigned int> depth; 
    unsigned int d;
    vector<unsigned int>::iterator it; 

    for (unsigned int i=0; i < plen/2; i++) {
        //depth first search for path
        vt.clear(); 
        dstack = stack<unsigned int>();
        depth = stack<unsigned int>();
        dstack.push(0); 
        depth.push(0); 
        mask = vector<bool>(mask_per); 
        while (!dstack.empty()) {
            v_curr = dstack.top(); 
            d = depth.top();
            dstack.pop(); 
            depth.pop(); 

            if (d < vt.size()) {
                // unset last element
                mask[vt[vt.size()-1]] = true; 
                vt.pop_back();
            }
            if (!mask[v_curr]) continue; 
            mask[v_curr] = false; 
            vt.push_back(v_curr); 
            if (d == plen) {
                if (clq[vt[vt.size()-1]][0] == 0) {
                    // back path exists, vt is sound
                    break; 
                }
            }
            for(it = clq[v_curr].begin(); it != clq[v_curr].end(); it++) {
                if (mask[*it]) {
                    dstack.push(*it); 
                    depth.push(d+1); 
                }
            }
        }

        paths.push_back(vt); 
        // remove all edges in vt; 
        for (j=0; j<plen; j++) {
            for(it = clq[j].begin(); it != clq[j].end(); it++) {
                if (*it == vt[j+1]) {
                    clq[j].erase(it); 
                    break; 
                }
            }
            for(it = clq[j+1].begin(); it != clq[j+1].end(); it++) {
                if (*it == vt[j]) {
                    clq[j+1].erase(it); 
                    break; 
                }
            }
        }
        for(it = clq[plen].begin(); it != clq[plen].end(); it++) {
            if (*it == vt[0]) {
                clq[j].erase(it); 
                break; 
            }
        }
        for(it = clq[0].begin(); it != clq[0].end(); it++) {
            if (*it == vt[plen]) {
                clq[0].erase(it); 
                break; 
            }
        }
    }
}

void dispath(graph &corr, clauses &cls, unsigned int plen) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    vector<unsigned int> vorder; 
    unsigned int n_ver = corr.get_nver(); 
    for(unsigned int i=0; i<n_ver; i++) vorder.push_back(i); 
    shuffle(vorder.begin(), vorder.end(), default_random_engine(seed)); 

    // for(unsigned int i=0; i<n_ver; i++) cout << vorder[i] << ' '; 
    // cout << endl;

    vector<vector<unsigned int>> clqpaths; 
    vector<unsigned int> path_t; 

    disall(clqpaths, plen); 
    path_t.resize(plen+1); 

    // cout << clqpaths.size() << endl; 

    for (unsigned int i = 0; i < (n_ver / (plen+1)); i++) {
        for (unsigned int k = 0; k < clqpaths.size(); k++) {
            for (unsigned int j = 0; j <= plen; j++) {
                path_t[j] = vorder[i*(plen+1) + clqpaths[k][j]]; 
            }
            // generate all cyclic of such paths
            ppath(cls, path_t); 
        }
    }
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

void eclus (graph &corr, clauses &cls, unsigned int plen) {
    eclus(corr, cls); 
    for (unsigned int i = 3; i <= plen; i++) 
        dispath(corr, cls, i); 
}
