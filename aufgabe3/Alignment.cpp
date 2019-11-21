#include "Alignment.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>

Alignment::Alignment(const std::string& seq_v, const std::string& seq_h){
    seq_v_ = seq_v;
    seq_h_ = seq_h;
    seq_v_size_ = seq_v_.size();
    seq_h_size_ = seq_h_.size();
    score_ = -100000;
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align){
   
    int diagonal = 1;
    int links = 2;
    int hoch = 3;
   
   if(local_align == true){
        //throw std::invalid_argument("Die Implementierung unterstuetzt kein smith-waterman");
        
        matrix_.clear();
        a1_.clear();
        a2_.clear();
        gaps_.clear();
        matrix_ = std::vector<std::pair<double,int>>( ((seq_v_size_+1) * (seq_h_size_+1)), (std::make_pair(0.0,NULL)) );
        
        //INITIALISIERUNG BEREITS PASSIERT!! DURH ART DER MATRIXERSTELLUNG sind zeile und spalte ein bereits mit 0,Null befüllt
        
        //Matrixbefüllung
        double tempMax = 0;
        int maxIndex = 0;
        for(int i = 0; i<matrix_.size();++i){
            if((i>seq_h_size_) && (i%(seq_h_size_ + 1)!=0)){
                double h = matrix_[i-(seq_h_size_ +1)].first +gap;
                double l = matrix_[i-1].first+gap;
                double d;
            
                if((seq_h_.at (( i % ( seq_h_size_+1 ))-1 ) ) ==( ( seq_v_.at ( (i/ (seq_h_size_+1 ) )-1) ) ) ) {
                    d = matrix_[(i-(seq_h_size_+2))].first + match;
                }else{
                    d =matrix_[(i-(seq_h_size_+2))].first + mismatch;
                }
            
                double max = fmax(fmax(fmax(h,l),d),0);
                matrix_[i].first = max; 
                if(max > tempMax){
                    tempMax = max; // MAXSCORE FÜR TRACEBACK START
                    maxIndex = i;
                }
                if(max == 0){
                    matrix_[i].second = NULL;
                    
                }else{
                if(d==max){
                    matrix_[i].second = 1;
                }else{
                    if(h==max){
                        matrix_[i].second = 3;
                    }else{
                        matrix_[i].second = 2;
                    }
                }
            }
            }
        }
    score_ = tempMax;
    
   int i = maxIndex;
    while(i > 0){
        if(matrix_[i].second == NULL){
            break;
        }else if(matrix_[i].second == 1){ //Diagonal
            //if-check for match or mismatch:
            if((seq_v_.at((i / (seq_h_size_ +1 )) -1 )) == (seq_h_.at(( i % (seq_h_size_ + 1)) -1 ))){  //better use div and quot to avoid float flooring 
                gaps_.push_back('|');
            }else{
                gaps_.push_back(' ');
            }
            a1_.push_back(seq_v_.at((i / (seq_h_size_ +1 )) -1 ));
            a2_.push_back(seq_h_.at(( i % (seq_h_size_ + 1)) -1 ));
            i = i - seq_h_size_ -2; 
        }else if(matrix_[i].second == 2){ //Links
            if(i <= seq_h_size_){
                a1_.push_back('-');
                a2_.push_back(seq_h_.at(i));
                gaps_.push_back(' '); 
            }else{
                a1_.push_back('-');
                a2_.push_back(seq_h_.at((i % (seq_h_size_+1 )) -1 ));
                gaps_.push_back(' ');
            }
            i = i - 1; 
        }else if(matrix_[i].second == 3){ //Hoch
            if(i % (seq_h_size_ + 1) == 0){ //falls wir uns am "linken Rand" der gemalten Matrix befinden
                a1_.push_back(seq_v_.at( i / (seq_h_size_+1) -1 ));
                a2_.push_back('-'); 
                gaps_.push_back(' ');
            }else{
                a1_.push_back(seq_v_.at(( i / (seq_h_size_ + 1)) -1 ));
                a2_.push_back('-'); 
                gaps_.push_back(' ');
            }
            i = i - seq_h_size_ -1; 
        }
    }

    std::reverse(a1_.begin(), a1_.end());
    std::reverse(a2_.begin(), a2_.end());
    std::reverse(gaps_.begin(), gaps_.end());
        
        
        
        
   }else{

    //ENDE SMITH-WATERMAN

    matrix_.clear();
    a1_.clear();
    a2_.clear();
    gaps_.clear();
    matrix_ = std::vector<std::pair<double,int>>( ((seq_v_size_+1) * (seq_h_size_+1)), (std::make_pair(0.0,NULL)) );
    
    
    //matrix_ INITIALISIERUNG
    int tempCount = 1;
    for(int i = 0; i<matrix_.size();++i){
        if(i<=seq_h_size_){
            matrix_[i].first = gap*i;
            matrix_[i].second = links;
        }else{
            if(i% (seq_h_size_+1)==0 && i!=0){
                matrix_[i].first = gap*tempCount;
                matrix_[i].second = hoch;
                tempCount+=1;
            }
        }    
    }
    matrix_[0].second = 0;
    
    //Matrixbefüllung
    for(int i = 0; i<matrix_.size();++i){
        if((i>seq_h_size_) && (i%(seq_h_size_ + 1)!=0)){
            double h = matrix_[i-(seq_h_size_ +1)].first +gap;
            double l = matrix_[i-1].first+gap;
            double d;
            
            if((seq_h_.at (( i % ( seq_h_size_+1 ))-1 ) ) ==( ( seq_v_.at ( (i/ (seq_h_size_+1 ) )-1) ) ) ) {
                d = matrix_[(i-(seq_h_size_+2))].first + match;
            }else{
                d =matrix_[(i-(seq_h_size_+2))].first + mismatch;
            }
            
            double max = fmax(fmax(h,l),d);
            matrix_[i].first = max; 
            if(d==max){
                matrix_[i].second = 1;
            }else{
                if(h==max){
                    matrix_[i].second = 3;
                }else{
                    matrix_[i].second = 2;
                }
            }
        }
    }
    score_ = matrix_[matrix_.size()-1].first;

    //###########Trace-Back check to complete strings and fill gap-string########
    //start push_backs in member-variables 
   int i = matrix_.size()-1;
    while(i > 0){
        if(matrix_[i].second == NULL){
            break;
        }else if(matrix_[i].second == 1){ //Diagonal
            //if-check for match or mismatch:
            if((seq_v_.at((i / (seq_h_size_ +1 )) -1 )) == (seq_h_.at(( i % (seq_h_size_ + 1)) -1 ))){  //better use div and quot to avoid float flooring 
                gaps_.push_back('|');
            }else{
                gaps_.push_back(' ');
            }
            a1_.push_back(seq_v_.at((i / (seq_h_size_ +1 )) -1 ));
            a2_.push_back(seq_h_.at(( i % (seq_h_size_ + 1)) -1 ));
            i = i - seq_h_size_ -2; 
        }else if(matrix_[i].second == 2){ //Links
            if(i <= seq_h_size_){
                a1_.push_back('-');
                a2_.push_back(seq_h_.at(i));
                gaps_.push_back(' '); 
            }else{
                a1_.push_back('-');
                a2_.push_back(seq_h_.at((i % (seq_h_size_+1 )) -1 ));
                gaps_.push_back(' ');
            }
            i = i - 1; 
        }else if(matrix_[i].second == 3){ //Hoch
            if(i % (seq_h_size_ + 1) == 0){ //falls wir uns am "linken Rand" der gemalten Matrix befinden
                a1_.push_back(seq_v_.at( i / (seq_h_size_+1) -1 ));
                a2_.push_back('-'); 
                gaps_.push_back(' ');
            }else{
                a1_.push_back(seq_v_.at(( i / (seq_h_size_ + 1)) -1 ));
                a2_.push_back('-'); 
                gaps_.push_back(' ');
            }
            i = i - seq_h_size_ -1; 
        }
    }

    std::reverse(a1_.begin(), a1_.end());
    std::reverse(a2_.begin(), a2_.end());
    std::reverse(gaps_.begin(), gaps_.end());  
    }  
}

int Alignment::score() const{
    //aus der befüllten matrix_ optimalen score extrahieren
    if( score_ == -100000 ){
        throw std::logic_error("Sie muessen via Compute-Fkt. erst das Allignment berechnen bevor es einen Score geben kann.");
    } else {
        return score_;
    }
}

void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const{
    //Produktion der alignment und gap strings anhand der Matirx aus compute und dem score aus score.
    if( score_ == -100000 ){
        throw std::logic_error("Sie muessen via Compute-Fkt. erst das Allignment berechnen bevor es einen Score geben kann.");
    }
    a1 = a1_;
    a2 = a2_;
    gaps = gaps_;
    std::cout << std::endl << a1_ << std::endl << gaps_ << std::endl << a2_ << std::endl ;
}

