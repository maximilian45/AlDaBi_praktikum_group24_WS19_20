#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "aufgabe2.hpp"

bool wholeComp(const std::string pat, const std::string text, int x, int y){   //bitte nur auf den Text referenzieren, um bei großen strings nicht aus dem speicher zu fliessen...
    //Funktion die zurück gibt ob das gesuchte pattern bzw Substr 1 lex. kleiner ist als Text      bzw Substr2
    // Falls x != 0 wird im Str2 erst ab stelle x gesucht
        for(int i = 0; i<pat.size();++i){
            if(pat[i+x]<=text[(y+i)]){
                if(pat[i+x]==text[(y+i)]){
                    if(i == pat.size()-1){
						if(text[y+i+1]!=*(text.end()-1)){
							return true;
						}
					}
                    continue;
                }else{
                    return true;
                }
            }else{
                return false;
            }
        }
        return true;
    }

int isPrefix(std::string query, std::string text, int pos){
	int count = 0;	
	for(uint32_t i = 0; i<query.size();++i){
		if(query[i]==text[pos+i]){
			count +=1;			
			continue;
		}else{
			return count;
		}
	}
	return count;
}

//Constructs a suffixarray, lexikographisch sorted
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


//start finding those hits via binary search
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits){
    //Fehlerabfang    
    if(query == ""){
	    std::cout << "Ein Leeres Pattern könnte überall sein..." << std::endl;
	    return;
    }
    if(query == text){
    	std::cout << "Pattern und Text Identisch, somit ist der Hit direkt an der ersten Position..." << std::endl;
	    hits.push_back(0);
        return;
    } //


    hits = {};
    uint32_t n = sa.size()-1; //equals text.size()-1
    uint32_t pat = query.size()-1; //pattern length 
    uint32_t lp = 0;
    uint32_t rp = n;
    uint32_t l = 0;
    int32_t r = n;
    uint32_t m = (l+r)/2;
    bool test = true;

//Searching lp via binary search 
    if(wholeComp(query, text, 0, sa[0])){
        lp = 0;
        std::cout << "if-case #1 -> lp = 0 \n";
    }else if(!wholeComp(query, text, 0, (sa[n]))){
            lp = n;
            std::cout << "if-case #2 -> lp = n aka sa.size()-1 \n";
    }else{
        //(l,r)=(1,n) is definded above.
        std::cout << "Das hier ist der Beginn der while ...\n";
        while(r-l>1){
            m = std::ceil(((double)l+(double)r)/2); //normally, two ints are floored, but pseudo code wants it ceiled
            std::cout << "while-loop -> m = " << m << std::endl;
            if(wholeComp(query, text, 0, sa[m])){
                r=m;
                std::cout << "while-loop -> if-case -> r = " << r << std::endl;
            }else{
                l=m;
                std::cout << "while-loop else-clause -> l = " << l << std::endl;
            }
        }
        lp=r;
        std::cout << "end of while loop -> lp = " << lp << std::endl;
    }
    std::cout << "\nSearching lp:\nDas hier ist r: " << r << std::endl;
    std::cout << "Das hier ist rp: " << rp << std::endl;
    std::cout << "Das hier ist lp: " << lp << std::endl;
    std::cout << "Das hier ist l: " << l << std::endl << std::endl;


l = 0;
r = n;
m=0;
//Searching rp via binary search 
    if(!wholeComp(query, text, 0, sa[n]) || (query.size() == isPrefix(query, text, sa[n]))){
		rp = n;
        //std::cout << "IF 1 von rp-border \n";
    }else if(wholeComp(query, text, 0, (sa[0])) && !(query.size() == isPrefix(query, text, sa[0]))){
        rp = 0;
    }else{
        //(l,r)=(1,n) is definded above.
        std::cout << "\n\nDas hier ist der Beginn der while ...\n";
        while(r-l>1){
            m = std::ceil(((double)l+(double)r)/2); //is it really ceiled? thought, cpp wound floor the result of a division over to unsigned ints.. 
            if(!(wholeComp(query, text, 0, sa[m])) || (query.size() == isPrefix(query, text, sa[m]))){ //alternative: NOTwholeCompf -> l=m, else r=m;
                std::cout << "find rp: if-abfrage in der while\n";
                l=m;
            }else{
                r=m;
            }
        }
        rp=l;
    }
    std::cout << "\nSearching rp:\nDas hier ist R: " << r << std::endl;
    std::cout << "Das hier ist rp: " << rp << std::endl;
    std::cout << "Das hier ist lp: " << lp << std::endl;
    std::cout << "Das hier ist L: " << l << std::endl;
}