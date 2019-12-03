#include <string>
#include <vector>
#include <omp.h> // not needed here, but in .cpp

#include "a4_util.h"
#include "BLAST_Neighborhood.hpp"
#include <cmath>
//Berechnet den SCore zweier kmere
int BLAST_Neighborhood::scoring(const ScoreMatrix& matrix, std::string s1, std::string s2){
    int score = 0;
    for(int i = 0; i< s1.size(); ++i){
        score += matrix.score(s1[i],s2[i]);
    }
    return score;
}
//
void BLAST_Neighborhood::getAllWords(std::string kmer, int sizeW, std::vector<std::string>& allWords) {
    if (sizeW == 0) {
        allWords.push_back(kmer);
        return;
    }
    for (int i=0; i<20; ++i) {
        std::string tmpWord = kmer;
        tmpWord.push_back(toAminoAcid(i));
        getAllWords(tmpWord, sizeW-1, allWords);
    }
}


std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                           const ScoreMatrix& matrix,
                                           const int word_size,
                                           const int score_threshold,
                                           const int threads){
    if(word_size < 1){
        throw std::logic_error("Die Größe der gesuchten Nachbarn darf nicht kleiner als 1 sein.");
    }/*
    if(threads != 1){// MUSS NACH EINFÜGEN DER PARAL. RAUSGENOMMEN WERDEN!!!!!!!!!!!!!!
        throw std::logic_error("Diese Implementierung unterstützt keine Paralellsierung!");
    }*/
    
    std::vector<NHResult> result(query.size()-(word_size-1));
   // result.clear();
    if((query.size()) < word_size){
        return result;
    }
    
    //Bilden aller Infixe der länge word_size und speichern in result als NHResults
    
    #pragma omp parallel for num_threads(threads)
    for(int i = 0; i<=query.size()-word_size;++i){
        NHResult tmp;//kann evt aus schleife raus
        tmp.infix = query.substr(i,word_size);
        result[i] = tmp;
    }
    
    /*
    
    //RESULT.INFIX PRINT!!!!!
    for(int i = 0; i< result.size(); ++i){
        std::cout << "Result von " << i << ": " << scoring(matrix,result[i].infix,result[i].infix) << std::endl;
    }*/
    
    //Generieren alles möglichen kmere
    std::vector<std::string> allWords;
    getAllWords("",word_size,allWords);

    //ALLWORDS PRINT
    /*
    for(int i = 0; i< allWords.size();++i){
        std::cout << allWords[i] << " " << i<<std::endl;        
    } */   



   //Merken welche der Infixe nicht übreprüft werden soll, da sie im match mit sich selber nicht über den Treshhold kommen
   std::vector<int> ausschuss;
   for(int i = 0; i< result.size();++i){
        if(scoring(matrix,result[i].infix,result[i].infix) < score_threshold){
            ausschuss.push_back(0);
        }else{
            ausschuss.push_back(1);
        }
   }
   
    //PARALELLSTART!!!! #2 #pragma omp barrier
    #pragma omp parallel for num_threads(threads)
    for(int i = 0; i < result.size(); ++i){
        NHResult tmp = result[i];
        std::vector< std::pair <std::string, int> > tmpNeighbours;
        if(ausschuss[i] == 0){
            continue;
        }else{
            for(int j = 0; j<allWords.size();++j){
                int tmpScore = scoring(matrix, tmp.infix,allWords[j]);
                if(tmpScore >= score_threshold){
                    tmpNeighbours.push_back(make_pair(allWords[j],tmpScore)); // Müsste gehen, da ja jeder thred sich die tmp Variablen selber erstellt                }
                } 
        }
        //#pragma omp barrier
        result[i].neighbours = tmpNeighbours;
        }
    }
    
    
    
   // std::cout<<"I!!!!"<<std::endl;
    
                                           
    return result;
}
