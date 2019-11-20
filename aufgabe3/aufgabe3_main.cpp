#include "Alignment.hpp"
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]){
    //###########new main#######
    if(argc != 3 && argc != 6){
        std::cout << "Geben sie entweder nur die zu alignierenden Sequenzen an oder zusätzlich die Match,mismatch und gapkosten!" << std::endl;
        return 1;
    }
    if(argc == 3){
        std::string a1 = "";
        std::string a2 = "";
        std::string gaps = "";

        Alignment align{argv[1],argv[2]};
        align.compute(3, -1, -2);
        align.getAlignment(a1, gaps, a2);
        std::cout << "score:" << align.score();
        return 0;
    }
    if(argc == 6){
        std::string a1 = "";
        std::string a2 = "";
        std::string gaps = "";

        Alignment align{argv[1],argv[2]};
        align.compute(std::atoi(argv[3]),std::atoi(argv[4]),std::atoi(argv[5]));
        align.getAlignment(a1, gaps, a2);
        std::cout << "score:" << align.score() << std::endl;
        return 0;
    }



    
    return 0;
}
