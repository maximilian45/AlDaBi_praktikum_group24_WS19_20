#include "aufgabe2.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){

    std::vector<uint32_t> sa;
	std::vector<uint32_t> hits; 

    if(argc == 1){
        std::cout<<"Unexpected Input!!"<<std::endl;
        return 1;
    }
    std::string si ="si";
std::string mi = "mississippi";
    find(si,sa,mi,hits);
    return 0;

}

    /*construct(sa, argv[1]); 
    std::cout<<sa.size();   
    
    for(int i = 0; i<sa.size();i++){
       // std::cout<<"I: "+ i; 
        std::cout <<  sa[i]<< std::endl;
    }*/
