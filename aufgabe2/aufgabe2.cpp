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

//lcp-Berechnung:
int lcp(std::string text1, std::string text2, int x, int y){
	int count = 0;
	if((text1.size()-x)>= (text2.size()-y)){
		std::string::iterator it=text2.begin();
		for(uint32_t i = x; i < (text2.size()-y) ; ++i){
			if(it != text2.end()){			
				if(text1[i]==text2[i+y]){
					count +=1;
					it+=1;
					continue;
				}else{break;}
			}else{
				break;
			}
		}
	}else{
		for(uint32_t i = y; i<(text1.size()-x);++i){
			std::string::iterator it=text1.begin();
			if(it!=text1.end()){			
				if(text1[i+x]==text2[i]){
					count += 1;
					++it;
					continue;
				}else{break;}
			}else{
				break;
			}		
		}
	}

	return count;
}
//



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

//variable-definition:
    hits = {};
    uint32_t n = sa.size()-1; //equals text.size()-1
    uint32_t pat = query.size()-1; //pattern length

    int32_t lp = 0;
    uint32_t rp = n;
    uint32_t l = 0;
    uint32_t r = n;
    uint32_t middle = 0;      //= ceil(((double)l+(double)r)/2);
    
    uint32_t lcpL = 0;
    uint32_t lcpR = 0;
    uint32_t m = 0; //amount
    uint32_t mlr = std::min(lcpL,lcpR);
//

//Searching lp via binary search 


    if(wholeComp(query, text, 0, sa[0])){
        if((pat+1)==isPrefix(query, text, sa[0])){ //wenn lcps berechnet werden können, kann man das ersetzen mit:
        //if((lcp(query, text, 0, sa[0])) == pat /*pat+1*/){
            lp = 0;
        }else{
            lp = -1;
        }
        //std::cout << "if-case #1 -> lp = 0 \n";
    }else if(!wholeComp(query, text, 0, (sa[n]))){
            lp = n;
            std::cout << "if-case #2 -> lp = n aka sa.size()-1 \n";
    }else{
        //(l,r)=(1,n) is definded above.
        //std::cout << "Das hier ist der Beginn der while ...\n";
        while(r-l>1){
            middle = std::ceil(((double)l+(double)r)/2); //normally, two ints are floored, but pseudo code wants it ceiled
            //std::cout << "while-loop -> m = " << m << std::endl;
            if(wholeComp(query, text, 0, sa[middle])){
                r=middle;
                //std::cout << "while-loop -> if-case -> r = " << r << std::endl;
            }else{
                l=middle;
                //std::cout << "while-loop else-clause -> l = " << l << std::endl;
            }
        }
        lp=r;
        //std::cout << "end of while loop -> lp = " << lp << std::endl;
    }
    /*
    std::cout << "\nSearching lp:\nDas hier ist r: " << r << std::endl;
    std::cout << "Das hier ist rp: " << rp << std::endl;
    std::cout << "Das hier ist lp: " << lp << std::endl;
    std::cout << "Das hier ist l: " << l << std::endl << std::endl;
    */

l = 0;
r = n;
middle=0;
//Searching rp via binary search 
    if(query.size() == isPrefix(query, text, sa[n])){
		rp = n;
    }else if(!wholeComp(query, text, 0, sa[n])){
        rp = n+1;
        //std::cout << "IF 1 von rp-border \n";
    }else if(wholeComp(query, text, 0, (sa[0])) && !(query.size() == isPrefix(query, text, sa[0]))){
        rp = 0;
    }else{
        //(l,r)=(1,n) is definded above.
        //std::cout << "\n\nDas hier ist der Beginn der while ...\n";
        while(r-l>1){
            middle = std::ceil(((double)l+(double)r)/2); //is it really ceiled? thought, cpp wound floor the result of a division over to unsigned ints.. 
            if(!(wholeComp(query, text, 0, sa[middle])) || (query.size() == isPrefix(query, text, sa[middle]))){ //alternative: NOTwholeCompf -> l=m, else r=m;
                //std::cout << "find rp: if-abfrage in der while\n";
                l=middle;
            }else{
                r=middle;
            }
        }
        rp=l;
    }
    
    std::cout << "\nSearching rp:\nDas hier ist R: " << r << std::endl;
    std::cout << "Das hier ist rp: " << rp << std::endl;
    std::cout << "Das hier ist lp: " << lp << std::endl;
    std::cout << "Das hier ist L: " << l << std::endl;

//set hit-vector up:
    hits.clear();
    if((lp >= 0) && (rp <= n) && (lp <= rp)){
        uint32_t irre = 0;
        for(uint32_t c = lp; c <= rp; ++c){
            hits.push_back(sa[c]);
            std::cout << hits[irre] << std::endl;
            irre +=1; 
        }
    std::sort(hits.begin(), hits.end());
    }

    //This is just for testing, should go in main !!!
    std::cout << "\n\n Gefundene Hits: \n";
    for(int i = 0; i < hits.size();++i){
        std::cout << hits[i] << std::endl; 
    }

}