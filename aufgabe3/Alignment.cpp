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
        
   }

    int diagonal = 1;
    int links = 2;
    int hoch = 3;
    
    //Matrixinitialisierung
    //
    //matrix_version mit einem Vector --->  Size abfragen funktionieren nicht.
    //std::vector<std::pair<int,int>> matrix_( ((seq_v_size_+1) * (seq_h_size_+1)), (std::make_pair(0,NULL)) );
   // matrix_ = matrix;
    //Implementierung mit 2 verschachtelten vectoren
    //std::vector<std::pair<int,int>> rows(seq_h_.size(),(std::make_pair(0,NULL)));
    //std::vector<std::vector<std::pair<int,int>>> matrix_(seq_v_.size(),rows);
    
   // PRINTOUT SAMPLE
    std::cout<<"matrix_size: " <<matrix_.size()<<std::endl;
    matrix_[2*3+1] = std::make_pair(1,2);
    for(int i = 0; i<matrix_.size(); ++i){
        if(i% (seq_h_size_+1)==0){std::cout<<std::endl;}
        std::cout<<matrix_[i].first;
    } 
    std::cout << std::endl;
    
    
    
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
    
    
    
    //Matrixbefüllung
    
    for(int i = 0; i<matrix_.size();++i){
        if((i>seq_h_size_) && (i%(seq_h_size_ + 1)!=0)){
            double h = matrix_[i-(seq_h_size_ +1)].first +gap;
            double l = matrix_[i-1].first+gap;
            double d ;
            std::cout<<"I: " << i <<std::endl;
            std::cout<<"I/: " << i/(seq_h_size_+1) <<std::endl;
            std::cout<<"I%: " <<(i%(seq_h_size_+1)) <<std::endl;
            
            
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
    
    
        for(int i = 0; i<matrix_.size(); ++i){
        if(i% (seq_h_size_+1)==0){std::cout<<std::endl;}
        std::cout<<matrix_[i].first<<" ";
    }
     
    
}


int Alignment::score() const{
    //aus der befüllten matrix_ optimalen score extrahieren
}

void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const{
    //Produktion der alignment und gap strings anhand der Matirx aus compute und dem score aus score.
}

