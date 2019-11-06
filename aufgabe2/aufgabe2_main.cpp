#include "aufgabe2.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){

    std::vector<uint32_t> sa;
	std::vector<uint32_t> hits = {}; 

    if(argc == 1){
        std::cout<<"Unexpected Input!!"<<std::endl;
        return 1;
    }
    /*
    std::string si ="si";
    std::string mi = "mississippi";
    find(si,sa,mi,hits);
    */

    std::string text = argv[1];
    std::cout << text << std::endl;
    std::string query = argv[2];
    std::cout << query << std::endl; 

    construct(sa, argv[1]); 
    std::cout<<"\nSuffix-Array.size():\n" << sa.size();   
    std::cout << "\nArray:"<< std::endl;
    for(int i = 0; i<sa.size();++i){
        std::cout <<  sa[i]<< std::endl;
    }
    find(query, sa, text, hits);


    return 0;
}