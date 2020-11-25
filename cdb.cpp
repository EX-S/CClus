// cdb.cpp

#include <iostream>
#include <vector>
#include <cmath>

#include "cdb.h"

using namespace std;


clauses::clauses() {
    this->size = 0;
    this->n_var = 0;
    this->wtotal = 0;
}

bool clauses::is_empty() {
    return !size;
}

unsigned long clauses::get_size() {
    return size;
}

void clauses::set_nvar(unsigned long nvar){
    n_var = nvar;
}

unsigned long clauses::get_nvar() {
    return n_var; 
}

unsigned long long clauses::get_wtotal() {
    return this->wtotal; 
}

void clauses::add(clause cl) {
    cls.push_back(cl); 
    size++;
}

void clauses::add(clause cl, unsigned int w) {
    cls.push_back(cl); 
    weights.push_back(w); 
    smarker.push_back(this->size); 
    size++;
    wtotal += w;
}

void clauses::pop() {
    if(size) {
        cls.pop_back(); 
        size--;
        
        if(*smarker.end() == size) {
            wtotal -= *weights.end(); 
            weights.pop_back(); 
            smarker.pop_back(); 
        }
    }
}

clause& clauses::operator[](unsigned long index) {
    return cls[index];
}

// void clauses::append(clauses& cls_n) {
//     cls.insert(cls.end(), cls_n.begin(), cls_n.end()); 
//     size+=cls_n.get_size(); 
// }

void clauses::print(ostream &os) {
    unsigned long long top = wtotal+1;
    unsigned long m_curr = 0; 
    os << "p wcnf " << n_var << ' ' << size << ' ' << top << endl; 
    for (unsigned long i = 0; i < size; i++) {
        if (i == smarker[m_curr]) {
            os << weights[m_curr]; 
            m_curr++;
        } else
            os << top; 

        for (unsigned long j = 0; j < cls[i].size(); j++)
            os << ' ' << cls[i][j];  
        os << ' ' << 0 << endl; 
    }
}

void clauses::print() {
    this->print(cout); 
}
