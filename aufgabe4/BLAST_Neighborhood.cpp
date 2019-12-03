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

void BLAST_Neighborhood::getAllWords(std::string kmer, int sizeA, int sizeW, std::vector<std::string>& allWords) {
    if (sizeW == 0) {
        allWords.push_back(kmer);
        return;
    }
    for (int i=0; i<sizeA; ++i) {
        std::string tmpWord = kmer;
        tmpWord.push_back(toAminoAcid(i));
        getAllWords(tmpWord, sizeA, sizeW-1, allWords);
}


//rekursive Nachbargenerierung
std::vector<std::pair<std::string,int>> BLAST_Neighborhood::genNeigh(NHResult input, int kmer,const ScoreMatrix& matrix,const int score_threshold){
    //Übergehen der Worte die im Slebstvergleich unter dem Treshhold landen, da dies der fürs Wort der maximale Score wert wäre
    if(scoring(matrix,input.infix,input.infix) < score_threshold){
        return std::vector<std::pair<std::string,int>> output;
    }
    

    //Mögliche ankervariante
    if(kmer = 1){
        
        
    }else{
        //Berechnung aller möglichen kmere und deren SCores
           
        
        
        
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
    if((query.size()-1) < word_size){
        return result;
    }
    
    //Bilden aller Infixe der länge word_size und speichern in result als NHResults
    
    #pragma omp parallel for num_threads(threads)
    for(int i = 0; i<=query.size()-word_size;++i){
        NHResult tmp;//kann evt aus schleife raus
        tmp.infix = query.substr(i,word_size);
        result[i] = tmp;
    }
    
    
    
    //RESULT.INFIX PRINT!!!!!
    for(int i = 0; i< result.size(); ++i){
        std::cout << "Result von " << i << ": " << scoring(matrix,result[i].infix,result[i].infix) << std::endl;
    }
    
    //Generieren alles möglichen kmere
    std::vector<std::string> allWords;
    getAllWords("",20,word_size,allWords);
    
    //PARALELLSTART!!!!
   
  /* for(int l = 0; l<result.size();++l){ 
        
        for(int i = 0;i<20;++i){
            NHResult tmp;
            for(int j = 0; j< 20 , ++j){
                for(int k = 0; k<20; ++k){
                    std::string tmpS;
                    tmpS += toAminoAcid(i);
                    tmpS += toAminoAcid(j);
                    tmpS += toAminoAcid(k);
                
                }
            }
        }
    }*/
    
    for(int i = 0; i < result.size(); ++i){
        NHResult tmp = result[i];
        
    }
    
    
    
   // std::cout<<"I!!!!"<<std::endl;
    
                                           
    return result;
}
