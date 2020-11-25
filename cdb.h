#ifndef CDB_H
#define CDB_H

#include <iostream>
#include <vector>

typedef std::vector<long> clause;

struct clauses {
    unsigned long size; 
    unsigned long n_var; 
    unsigned long long wtotal; 
    std::vector<clause> cls; 
    std::vector<unsigned int> weights; 
    std::vector<unsigned long> smarker; 

    clauses(); 

    bool is_empty(); 
    unsigned long get_size(); 
    void set_nvar(unsigned long nvar); 
    unsigned long get_nvar();
    unsigned long long get_wtotal(); 

    void add(clause cl);
    void add(clause cl, unsigned int w);
    void pop();
    // void append(clauses& cls_n); 

    clause& operator[](unsigned long index); 

    void print(std::ostream &os); 
    void print(); 
}; 

#endif
