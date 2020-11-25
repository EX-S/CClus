#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <set>

struct edgew {
    unsigned int ver; 
    unsigned int weight; 

    edgew(); 
    edgew(unsigned int v); 
    edgew(unsigned int v, unsigned int w); 
    bool operator< (const edgew &rhs); 
};

bool operator< (const edgew &lhs, const edgew &rhs); 

struct graph {
    unsigned int n_ver; 
    unsigned int size; 
    std::vector<std::set<edgew>> adjlist; 

    graph(); 
    graph(unsigned int nver); 

    bool is_empty(); 
    void set_nver(unsigned int nver);
    unsigned int get_nver(); 
    unsigned int get_size(); 

    void insert(unsigned int v1, unsigned int v2, int w); // order depends on w
    unsigned int get_weight(unsigned int v1, unsigned int v2); // order matters
    // unsigned int get_degree(unsigned int v); 
    // unsigned int get_degree(unsigned int v, bool is_pos); 
    
    void subgraph(graph &mysub, unsigned int nver); 
    
    void print(std::ostream &os); 
    void print(); 
};

#endif
