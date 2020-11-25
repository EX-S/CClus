#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>

#include "clus_handle.h"
#include "eclus.h"
#include "cdb.h"
#include "graph.h"

using namespace std;


int main(int argc, char **argv) {
    if (argc < 4) {
        cout << "Usage: cclus <input> <output> <type> [option]" << endl; 
        return 1;
    }

    // string line; 

    ifstream fi;
    ofstream fo; 
    ofstream flog;
    fi.open(argv[1], ifstream::in); 
    fo.open(argv[2], ofstream::out | ofstream::trunc); 
    flog.open("cclus.log", ofstream::out | ofstream::app); 

    if (!fi.is_open()) {
        cout << "Error opening input file" << argv[1] << endl; 
    }
    if (!fo.is_open()) {
        cout << "Error opening output file" << argv[2] << endl;
    }
    if (!flog.is_open()) {
        cout << "Error opening log file" << "log" << endl;
    }

    int vtx;
    long rel; 

    // string v1, v2; 
    unsigned int vn1, vn2;
    int weight; 

    // map<string, unsigned int> vton; 
    // map<string, unsigned int>::iterator mit; 

    graph corr; 
    clauses cls;

    
    flog << "################################################################################" << endl;  
    
    fi >> vtx; 
    fi >> rel;

    flog << "Number of Vertices: " << vtx << endl;
    flog << "Non-zero Correlations: " << rel << endl; 

    flog << "================================================================================" << endl;  

    // init 
    corr.set_nver(vtx); 

    for (int i = 0; i < rel; i++) {
        fi >> vn1;
        fi >> vn2;
        fi >> weight;

        // vn1 = get_vnum(vton, v1); 
        // vn2 = get_vnum(vton, v2); 

        if(vn1 == vn2) continue; 

        corr.insert(vn1, vn2, weight); // no need to resize
    }

    if (strcmp(argv[3], "eclus") == 0) {
        if (argc == 4) {
            eclus(corr, cls); 
        }
        // TODO: add extra options
    }
    // TODO: add vertex based
    
    cls.print(fo); 
    
    flog << "Number of Variables: " << cls.get_nvar() << endl; 
    flog << "Number of Clauses: " << cls.get_size() << endl; 

    flog << "End" << endl;  
    flog << "################################################################################" << endl;  
    

    fi.close(); 
    fo.close();

    return 0;
}
