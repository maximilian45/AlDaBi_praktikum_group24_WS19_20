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

    std::vector<NHResult> result;
    if(word_size < 1){
        throw std::logic_error("Die Größe der Worte darf nicht kleiner als 1 sein.");
    }
    if(threads < 1){
        throw std::logic_error("Eine Ausfuehrung benoetigt mindestens einen Thread.");
    }
    if((query.size()) < word_size){
        //throw std::logic_error("Die Größe Query darf nicht kleiner als die gewuenschte Wortgroesse sein.");
        //aber das Testprogramm will hier keinen Error, sondern result returned haben.
        return result;
    }
    
    //Bilden aller Infixe der länge word_size und speichern in result als NHResults
    result.resize(query.size()-(word_size-1));

    #pragma omp parallel for num_threads(threads)
    for(int i = 0; i<=query.size()-word_size;++i){
        NHResult tmp;
        tmp.infix = query.substr(i,word_size);
        result[i] = tmp;
    }
    
    //Generieren aller möglichen kmere
    std::vector<std::string> allWords;
    getAllWords("",word_size,allWords);   

   //Merken welche der Infixe nicht übreprüft werden soll, da sie im match mit sich selber nicht über den Treshhold kommen
   std::vector<int> ausschuss;
   for(int i = 0; i< result.size();++i){
        if(scoring(matrix,result[i].infix,result[i].infix) < score_threshold){
            ausschuss.push_back(0);
        }else{
            ausschuss.push_back(1);
        }
   }
   
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
return result;
}