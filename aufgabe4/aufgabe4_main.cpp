#include "a4_util.h"
#include "BLAST_Neighborhood.hpp"
#include <string>
#include <cstdlib>
#include <omp.h>


int main(int argc, char* argv[]){
    
    if(argc != 6){
        std::cout << argc << "Fehlerhafte Eingabe. Bitte folgende Argumente Übergeben: Sequenz, Matrixname, Querylength,Treshhold, Thredanzahl"<< std::endl;
        return 1;
    }
    else{
        ScoreMatrix matrix;
        
        try{
        matrix.load(argv[2]);
        }
        catch(IOException){
            std::cout << "Matrix konnte nicht geladen werden. Bitte Eingabe überprüfen!"<<std::endl;
            return 1;
        }
        std::string temp = argv[1];
        if(temp.size() < atoi(argv[3])){
            return 1;
        }
        
        BLAST_Neighborhood test;
        double startZeit = omp_get_wtime();
        
        std::cout << "START "<< startZeit<<std::endl;
        std::vector<NHResult> result = test.generateNeighborhood(argv[1],matrix,atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
        double endZeit = omp_get_wtime() - startZeit;
        
        for(int i = 0; i< result.size();++i){
            std::cout << result[i].infix << ": ";
            for(int j = 0; j < result[i].neighbours.size() ; ++j){
                std::cout << "(" << result[i].neighbours[j].first << ", " << result[i].neighbours[j].second << ") ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "TIME: " << endZeit<<std::endl;
        result.clear();
    }
    
    return 0;
}
