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
    matrix_ = std::vector<std::pair<double,int>>( ((seq_v_size_+1) * (seq_h_size_+1)), (std::make_pair(0.0,NULL)) );
}

void Alignment::compute(const int match, const int mismatch, const int gap, const bool local_align){
   
   if(local_align == true){
        throw std::invalid_argument("Die Implementierung unterstuetzt kein smith-waterman");
        return;
   }

    int diagonal = 1;
    int links = 2;
    int hoch = 3;    
    
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
    
   /* 
   // PRINTOUT SAMPLE after INIT
    //std::cout<<"matrix_size: " <<matrix_.size()<<std::endl;
    //matrix_[2*3+1] = std::make_pair(1,2);
    for(int i = 0; i<matrix_.size(); ++i){
        if(i% (seq_h_size_+1)==0){std::cout<<std::endl;}
        std::cout<<matrix_[i].first;
    } 
    std::cout<<"Tracebackmatrix_:" <<std::endl;
    
    for(int i = 0; i<matrix_.size(); ++i){
        if(i% (seq_h_size_+1)==0){std::cout<<std::endl;}
        std::cout<<matrix_[i].second;
    }
    */
    
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

    ///Wenn die matrix am ende gecleared wird, ist diese Zuweisung fuer die score()-funktion essentiell:
    score_ = matrix_[matrix_.size()-1].first;

    //Matrix-Print-Out:
    std::cout << "Score-Matrix: " << std::endl;
    for(int i = 0; i < matrix_.size(); ++i){
        if(i% (seq_h_size_+1)==0){std::cout<<std::endl;}
    std::cout<<matrix_[i].first<<" ";
    }


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

    std::cout << std::endl << a1_ << std::endl << gaps_ << std::endl << a2_ << std::endl ;

    matrix_.clear();

}


int Alignment::score() const{
    //aus der befüllten matrix_ optimalen score extrahieren
    if(matrix_[matrix_.size()-1].second == NULL){
        throw std::logic_error("Sie muessen via Compute-Fkt. erst das Allignment berechnen bevor es einen Score geben kann.");
    } else {
        return matrix_[matrix_.size()-1].first;
    }
}

void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const{
    //Produktion der alignment und gap strings anhand der Matirx aus compute und dem score aus score.
    if(matrix_[matrix_.size()-1].second == NULL){
        throw std::logic_error("Sie muessen via Compute-Fkt. erst das Allignment berechnen bevor es einen Score geben kann.");
    }
    
}

