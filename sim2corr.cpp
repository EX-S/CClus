#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <map> 
#include <vector>
#include <set>

#include "graph.h"
#include "clus_handle.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "Usage: sim2corr <input.sim> <output.corr> [sub_size]"; 
    }

    ifstream fi;
    ofstream fo; 
    stringstream ss; 

    fi.open(argv[1], ifstream::in); 
    fo.open(argv[2], ofstream::out | ofstream::trunc); 

    unsigned int subver = 0xffff; 

    string line; 
    string v1, v2; 
    unsigned int vn1, vn2; 

    double simw;
    int w; 

    map<string, unsigned int> vton; 
    map<string, unsigned int>::iterator mit; 

    graph corr; 
    graph subcorr;

    if (argc == 4) {
        ss << argv[3]; 
        ss >> subver; 
    }

    while(getline(fi, line)) {
        if (line[0] != '%') break; 
    }

    do {
        ss = stringstream(line); 
        ss >> v1;
        ss >> v2; 
        ss >> simw; 

        vn1 = get_vnum(vton, v1); 
        vn2 = get_vnum(vton, v2); 

        if(vn1 == vn2) continue; 

        simw = simw*2 -  1; 
        w = (int)round(simw*0x1000); 

        if(corr.get_nver() < vton.size()) // resize if needed
            corr.set_nver(vton.size()); 
        corr.insert(vn1, vn2, w); 
    } while (getline(fi, line)); 

    if (corr.get_nver() > subver) {
        corr.subgraph(subcorr, subver); 
        subcorr.print(fo); 
    } else 
        corr.print(fo);
    
    fi.close();
    fo.close();

    return 0;
}
