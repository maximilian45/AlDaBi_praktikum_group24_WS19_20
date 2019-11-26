#include <string>
#include <vector>
#include <omp.h> // not needed here, but in .cpp

#include "a4_util.h"
#include "BLAST_Neighborhood.hpp"


std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                           const ScoreMatrix& matrix,
                                           const int word_size,
                                           const int score_threshold,
                                           const int threads){
    if(word_size < 1){
        throw std::logic_error("Die Größe der gesuchten Nachbarn darf nicht kleiner als 1 sein.");
    }
    if(threads != 1){// MUSS NACH EINFÜGEN DER PARAL. RAUSGENOMMEN WERDEN!!!!!!!!!!!!!!
        throw std::logic_error("Diese Implementierung unterstützt keine Paralellsierung!");
    }                                           
    
    std::vector<NHResult> result;
    if((query.size()-1) < word_size){
        return result;
    }
    
    //Bilden aller Infixe der länge word_size
    
    
    
    
   // std::cout<<"I!!!!"<<std::endl;
    
                                           
    return result;
}
