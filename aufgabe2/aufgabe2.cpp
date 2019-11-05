#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "aufgabe2.hpp"

/*
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

} */

bool wholeComp(const std::string pat, const std::string text, int x, int y){   //bitte nur auf den Text referenzieren, um bei großen strings nicht aus dem speicher zu fliessen...
    
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
		std::sort(sa.begin(), sa.end(), [text](uint32_t x, uint32_t y) {  //lambda, aber bitte nur auf den Text referenzieren, um bei großen strings nicht aus dem speicher zu fliessen...
			return wholeComp(text,text,x,y);
		});

}



void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits){

    //Fehlerabfang    
    if(query == ""){
	    std::cout << "Ein Leeres Pattern könnte überall sein..." << std::endl;
	    return;
    }
    if(query == text){
    	std::cout << "Pattern und Text Identisch" << std::endl;
	    return;
    }
    
    hits = {};
    uint32_t lp = 0;
    uint32_t rp = sa.size()-1;
    uint32_t l = 0;
    uint32_t r = sa.size()-1;
    uint32_t m = (l+r)/2;


//Searching lp via binary search 
    if(wholeComp(query, text, 0, sa[0])){
        lp = 0;
        std::cout << "IF 1 \n";
    }else if(wholeComp(query, text, 0, (sa[sa.size()-1]))){
            lp = text.length();
    }else{
        //(l,r)=(1,n) is definded above.
        std::cout << "Das hier ist der Beginn der while ...\n";
        while(l<r){
            m = (l+r)/2; //is it really ceiled? i thought, cpp wound floor the result of a division over to unsigned ints.. 
            if(wholeComp(query, text, 0, m)){
                r=m;
            }else{
                l=m;
            }
        }
        lp=r;
    }

std::cout << "\nDas hier ist L: " << l << std::endl;
std::cout << "\nDas hier ist R: " << r << std::endl;

l = 0;
r = sa.size()-1;
m=0;
//Searching rp via binary search 
    if(!wholeComp(query, text, 0, sa[0])){
        rp = text.length()-1;
        std::cout << "IF 1 von rp-border \n";
    }else if(wholeComp(query, text, 0, (sa[sa.size()-1]))){
            rp = 0;
    }else{
        //(l,r)=(1,n) is definded above.
        std::cout << "Das hier ist der Beginn der while ...\n";
        while(l<r){
            m = (l+r)/2; //is it really ceiled? thought, cpp wound floor the result of a division over to unsigned ints.. 
            if(!wholeComp(query, text, 0, m)){
                l=m;
            }else{
                r=m;
            }
        }
        rp=l;
    }
    std::cout << "\nDas hier ist R: " << r << std::endl;
    std::cout << "\nDas hier ist L: " << l << std::endl;
}
