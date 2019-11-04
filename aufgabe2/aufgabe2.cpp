#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "aufgabe2.hpp"

int comp(const std::string pat, const std::string text, int pos){
    int out = 0;
     //if(pat[0]==text[pos]){  
    int j = 0; 
    std::cout<<"HERE!!!!!";
    for(int i= pos; i<(pat.size()+pos);i++ ){
        if(((int)pat[j])==((int)text[pos])){
            
        }else{
            if(((int)pat[j])<((int)text[pos])){
                return -1;
            }else{
                return 1;
            }  
        j++; 
        }
    }
    return 0;

}

void construct(std::vector<uint32_t>& sa, const std::string& text){
        if(text == ""){
        	std::cout << "Im Leeren Text können wir nichts finden" << std::endl;
            sa.clear();
        	return;
        }        

        sa.clear();
    	sa.resize(text.length());
		size_t pos = 0;
		std::generate(sa.begin(), sa.end(), [&pos]() { return pos++; });
		std::sort(sa.begin(), sa.end(), [text](uint32_t x, uint32_t y) {
			return text.substr(x) < text.substr(y);
		});

}



void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits){

    //Fehlerabfang
   
    std::cout<<comp(query,text,3);
    
    if(query == ""){
	    std::cout << "Ein Leeres Pattern könnte überall sein..." << std::endl;
	    return;
    }
    if(query == text){
    	std::cout << "Pattern und Text Identisch" << std::endl;
	    return;
    }
    
    hits = {};
    
    uint32_t l = 0;
    uint32_t r = sa.size()-1;

    
    
    
    
}
