#include "aufgabe2.hpp"
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){

    std::vector<uint32_t> sa;
	std::vector<uint32_t> hits; 
	std::string test = "";


    if(argc == 1){
        std::cout<<"Unexpected Input!!"<<std::endl;
        return 1;
    }

	if(argv[1] == test){
		std::cout<< "In einem leeren String können wir nichts finden"<<std::endl;	
		return 0;
	}


if (argc == 2) {    
		construct(sa, argv[1]);
		for (unsigned int jv = 0; jv < sa.size(); ++jv) {
			std::cout << sa[jv] << "\n";
		}
		return 0;
	}

if (argc > 2) {
		construct(sa, argv[1]);
		for (int iv = 2; iv < argc; ++iv) {
			find(argv[iv], sa, argv[1], hits);
			if(argv[iv] != test ){
			std::cout << argv[iv] << ": ";
			for (unsigned int jh = 0;  jh < hits.size(); jh++) {
				std::cout << hits[jh] << " ";
			}}
			std::cout << "\n";
		}
	}
return 0;


}
