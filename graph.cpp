// graph.cpp

#include <iostream>
#include <vector>
#include <set>
#include <cmath>

#include "graph.h"

using namespace std;


edgew::edgew() {
    this->ver = 0;
    this->weight = 0;
}

edgew::edgew(unsigned int v) {
    this->ver = v;
    this->weight = 0;
}

edgew::edgew(unsigned int v, unsigned int w) {
    this->ver = v;
    this->weight = w;
}

bool edgew::operator< (const edgew &rhs) {
    return this->ver < rhs.ver; 
}

bool operator< (const edgew&lhs, const edgew &rhs) {
    return lhs.ver < rhs.ver; 
}


//==============================================================================

graph::graph() {
    this->n_ver = 0;
    this->size = 0;
}

graph::graph(unsigned int nver) {
    this->n_ver = nver; 
    this->size = 0;

    adjlist.resize(nver); 
}

bool graph::is_empty() {
    return size; 
}

void graph::set_nver(unsigned int nver) {
    this->n_ver = nver; 

    adjlist.resize(nver);
}

unsigned int graph::get_nver() {
    return n_ver; 
}

unsigned int graph::get_size() {
    return size; 
}

void graph::insert(unsigned int v1, unsigned int v2, int w) {
    unsigned int sml, big; 
    sml = min(v1, v2); 
    big = max(v1, v2); 
    if (w > 0) {
        adjlist[sml].insert(edgew(big,w));
        size++;
    } else if (w < 0) {
        adjlist[big].insert(edgew(sml,abs(w))); 
        size++;
    }
}

unsigned int graph::get_weight(unsigned int v1, unsigned int v2) {
    set<edgew>::iterator it; 
    edgew ew_t(v2); 
    it = adjlist[v1].find(ew_t); 
    if (it != adjlist[v1].end()) {
        return (*it).weight; 
    }
    return 0; 
}

void graph::subgraph(graph &mysub, unsigned int nver) {
    set<edgew>::iterator it; 

    mysub.set_nver(nver); 

    for(unsigned int i=0; i<nver; i++) {
        for(it = adjlist[i].begin(); it != adjlist[i].end(); it++) {
            unsigned int j = (*it).ver; 
            int w = (*it).weight; 
            if (j < nver) {
                if (i < j) // positive weight
                    mysub.insert(i, j, w); 
                else // negative weight
                    mysub.insert(i, j, -w); 
            }
        }
    }
}

void graph::print(ostream &os) {
    set<edgew>::iterator it; 
    os << n_ver << ' ' << size << endl; 
    for(unsigned int i=0; i<n_ver; i++) {
        for(it = adjlist[i].begin(); it != adjlist[i].end(); it++) {
            if (i < (*it).ver) {
                os << i << ' ' << (*it).ver << ' ' << (*it).weight << endl;
            } else if(i > (*it).ver) {
                os << i << ' ' << (*it).ver << ' ' << -(long)(*it).weight << endl; 
            }
        }
    }
}

void graph::print() {
    this->print(cout);
}
