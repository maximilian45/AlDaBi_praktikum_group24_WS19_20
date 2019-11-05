#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "aufgabe2.hpp"

int comp(const std::string pat, const std::string text, int pos){
    //Funktion die zwei Strings an der durch Pos angegebenen Stelle Gleichen oder nicht
    //Bei gleichheit wird 0, bei pat<text wird -1 und bei pat>taxt 1 zuückgegeben    

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

bool wholeComp(const std::string pat, const std::string text, int x, int y){
    
    //Funktion die zurück gibt ob das gesuchte pattern bzw Substr 1 lex. kleiner ist als Text      bzw Substr2
    // Falls x != 0 wird im Str2 erst ab stelle x gesucht
    if(x==0 && y==0){
        for(int i = 0; i < pat.size();++i){
            if(pat[i]<=text[i]){
                if(pat[i]==text[i]){
                    continue;
                }else{
                    return true;
                }   
            }else{
                return false;
            }
        }
    }else{
        for(int i = 0; i<pat.size();++i){
            if(pat[i+x]<=text[(y+i)]){
                if(pat[i+x]==text[(y+i)]){
                    continue;
                }else{
                    return true;
                }
            }else{
                return false;
            }
        }
    }

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
			return wholeComp(text,text,x,y);
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
