#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "aufgabe2.hpp"
#include <cmath>

uint32_t comp(const std::string pat, const std::string text, uint32_t pos){
    //Funktion die zwei Strings an der durch Pos angegebenen Stelle Gleichen oder nicht
    //Bei gleichheit wird 0, bei pat<text wird -1 und bei pat>taxt 1 zuückgegeben    

    uint32_t out = 0;
     //if(pat[0]==text[pos]){  
    uint32_t j = 0; 
    std::cout<<"HERE!!!!!";
    for(uint32_t i= pos; i<(pat.size()+pos);i++ ){
        if(((uint32_t)pat[j])==((uint32_t)text[pos])){
            
        }else{
            if(((uint32_t)pat[j])<((uint32_t)text[pos])){
                return -1;
            }else{
                return 1;
            }  
        j++; 
        }
    }
    return 0;

}

bool wholeComp(const std::string pat, const std::string text, uint32_t x, uint32_t y){
    
	//FEHLENDE FÄLLE: pattern eins länger als nis dahin identes pattern
	//				  
	
    //Funktion die zurück gibt ob das gesuchte pattern bzw Substr 1 lex. kleiner gleich ist als Text      bzw Substr2
    // Falls y != 0 wird im Str2 erst ab stelle y gesucht
	//Falls x!= 0 ist pat der zu durchsuende text und text das pattern. Text wird ann ab stelle x durchsucht
    if(x==0 && y==0){
        for(uint32_t i = 0; i < pat.size();++i){
            if(pat[i]<=text[i]){
                if(pat[i]==text[i]){
					if(i == pat.size()-1){
						if(text[i+1]!=*(text.end()-1)){
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
    }else{
        for(uint32_t i = 0; i<pat.size();++i){
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
    }

    return true;
}

uint32_t isPrefix(std::string query, std::string text, uint32_t pos){
	uint32_t count = 0;	
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
   
    //std::cout<<comp(query,text,3);
	std::cout<<"SA1: "<<std::endl;
    
    if(query == ""){
	    std::cout << "Ein Leeres Pattern könnte überall sein..." << std::endl;
	    return;
    }
    if(query == text){
    	std::cout << "Pattern und Text Identisch" << std::endl;
	    return;
    }
    
    hits = {};
    
    uint32_t left = 0;
    uint32_t rp = sa.size()-1;
    uint32_t l = 0;
    uint32_t r = sa.size()-1;
    uint32_t m = 0;
	bool test = false;



    //Berechnung der Linken Grenze Lp via Binärsuche

   while((r-l)>1){
	uint32_t dist = r-l;
	if( dist == 0){
	std::cout<<"IF 1 LP L: "<< l <<std::endl;
	std::cout<<"M: " << m <<std::endl;
	std::cout<< "R: "<<r <<std::endl;
		
		test = true;
	}else{
		std::cout<<"M PRÄ CEIL: " << m <<std::endl;
std::cout<<"L: "<< l <<std::endl;
std::cout<< "R: "<<r <<std::endl;
		m = std::ceil(((double)l+(double)r)/(double)2);
std::cout<<"HIER!!!!!!!!!!!!!!!!!!!!!!!!"<< ceil(2*(3+4)/4)<<std::endl;
		std::cout<<"M POST CEIL: " << m <<std::endl;
		if(!wholeComp(query,text,0,sa[m])){
			if((query.size()==isPrefix(query,text,sa[m]))){
				
	std::cout<<"MIF: " << m <<std::endl;
				l=m;
			}else{			
				l = m;
				std::cout<<"IF 2 LP L: "<< l <<std::endl;
				
	std::cout<<"M: " << m <<std::endl;
				std::cout<< "R: "<<r <<std::endl;
			}
		}else{
				std::cout<<"ELSE 2 LP L: "<< l <<std::endl;
	std::cout<< "R1: "<<r <<std::endl;
	
	std::cout<<"M: " << m <<std::endl;
			r = m;
			std::cout<< "R2: "<<r <<std::endl;
		}
	}
}
left = r;


	std::cout<<"L: "<< l <<std::endl;
	std::cout<< "R: "<<r <<std::endl;
	std::cout<< "LEFT BORDER: " << left <<std::endl;

	std::cout<<"POST LP PRÄ RP"<<std::endl;
    //Berechnung der rechten Grenze rp
	uint32_t right = 0;
	l = 0;
	m = 0;
	r = sa.size()-1;
	uint32_t tLen = text.size()-1;
	uint32_t qLen = query.size()-1;
	test = false;
/*
	if(0<text.compare(sa[0],query.size(),query)){
		right = -1;
	}else{
		if(0>=text.compare(sa[sa.size()],query.size(),query)){
			right = sa.size();	
		}else{
			while((r-l)>1){
				m = (uint32_t)ceil((l+r)/2);
				if(0<text.compare(sa[m],query.size(),query)){
					r=m;
				}else{
					l=m;				
				}
			}
		}
	}
	right=l;
*/






/*	if(!wholeComp(query,text,0,sa[sa.size()-1])){right=sa.size()-1;}
	else{
		if(wholeComp(query,text,0,sa[0])){right = 0;}
		else{
			while(r-l>1){
				m = ceil((l+r)/2);
				if(!wholeComp(query,text,0,sa[m])){l=m;}
				else{r=m;}
			}
		right = l;
		}
	}*/



while((r-l)>1){
	uint32_t dist = r-l;
	m = std::ceil(((double)l+(double)r)/(double)2);
	if((!wholeComp(query,text,0,sa[m]))||(query.size()==isPrefix(query,text,sa[m]))){
		l=m;
	}else{
		r=m;
	}
}
right = l;






/*
while(!test){

	//if(wholeComp(query,text,0,0))

	uint32_t dist = r-l;
	if( (r-l)>1){
	std::cout<<"IF 1 LP L: "<< l <<std::endl;
	std::cout<< "R: "<<r <<std::endl;
		right = l;
		test = true;
	}else{
		m = std::ceil(((double)l+(double)r)/(double)2);
		if(!wholeComp(query,text,0,sa[m])||(query.size()==isPrefix(query,text,sa[m])){
			//if(!(query.size()==isPrefix(query,text,sa[m]))){
			//	r=m;
			//}else{			
			//	l = m;
			//	std::cout<<"IF 2 LP L: "<< l <<std::endl;
			//	std::cout<< "R: "<<r <<std::endl;
			//}
		}else{
				std::cout<<"ELSE 2 LP L: "<< l <<std::endl;
				std::cout<< "R1: "<<r <<std::endl;
			if(!(query.size()==isPrefix(query,text,sa[m]))){
				l=m;
			}else{
				l=m;
			}
			std::cout<< "R2: "<<r <<std::endl;
		}
	}
}*/



/*	while(!test){
		uint32_t dist = r-l;
		if( dist == 0){
			right = l;
			test = true;
		}else{
			m = ceil((l+r)/2);
			//std::string txt = (text.substr(m));
			//IsPrefix prüft ob txt nicht kürzer alws query
			//und 
			//bool isPrefix = !((tLen-m)< qLen) && 
			//bool isPrefix = !((txt.length() < query.length()) && !txt.compare(0, query.length(), query));
			if(!((wholeComp(query,text,0,sa[m])) ||(query.size()==isPrefix(query,text,sa[m]) ) ) ){
				std::cout<<"IF 2 LP L: "<< l <<std::endl;
							
				l = m+1;
				std::cout<< "L2: "<<l <<std::endl;	
			}else{
					if((!(wholeComp(query,text,0,sa[m])) &&(query.size()==isPrefix(query,text,sa[m]) ) )){
						l=m+1;

					}else{
						std::cout<< "R: "<<r <<std::endl;	
						r = m;
						std::cout<< "R2: "<<r <<std::endl;
					}	
			}
		}
	}*/


	std::cout<<"POST RP"<<std::endl;
	std::cout<<l <<std::endl;
	std::cout<<r <<std::endl;
	std::cout<<"RICHT BORDER: " << right <<std::endl;
	
    
    

    

    
    
    
    
}
