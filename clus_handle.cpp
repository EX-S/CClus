// clus_handle.cpp

#include <map>
#include <string>

#include "clus_handle.h"

using namespace std;

unsigned int get_vnum(map<string, unsigned int> &vton, string &ver) {
    map<string, unsigned int>::iterator mit; 
    mit = vton.find(ver); 
    if (mit != vton.end()) 
        return mit->second; 
    vton[ver] = vton.size(); 
    return vton[ver]; 
}
