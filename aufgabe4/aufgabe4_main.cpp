#include "a4_util.h"
#include "BLAST_Neighborhood.hpp"
#include <string>
#include <cstdlib>
#include <omp.h>


int main(int argc, char* argv[]){
    
    if(argc != 6){
        std::cout <<argc<<"Fehlerhafte Eingabe. Bitte folgende Argumente Übergeben: Sequenz, Matrixname, Querylength,Treshhold, Thredanzahl"<< std::endl;
        return 1;
    }
    else{
        ScoreMatrix matrix;
        
        try{
        matrix.load(argv[2]);
        matrix.print();
        }
        catch(IOException){
            //
            std::cout << "Matrix konnte nicht geladen werden. Bitte Eingabe überprüfen!"<<std::endl;
            return 1;
        }
        BLAST_Neighborhood test;
        int startZeit = omp_get_wtime();
        test.generateNeighborhood(argv[1],matrix,atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
        int endZeit = omp_get_wtime() - startZeit;
    }
    return 0;
}
