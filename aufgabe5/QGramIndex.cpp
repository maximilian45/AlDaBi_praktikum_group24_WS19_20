#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <math.h>
#include <algorithm>
#include<omp.h> 
#include "QGramIndex.hpp"
#include "a5_util.hpp"

QGramIndex::QGramIndex(const std::string& text, const uint8_t q):genomdata(text){ //build
    if(q<1||q>13){ //exception for handed q 
      throw std::invalid_argument("Invalid q!");
    } //initialize
	//this -> text = text;
	//genomdata.append(text);
    querylength = q;
    buildSuftab();
}

const std::string& QGramIndex::getText() const { //return the data, but fails to update genomedata 'cause the old text is in it kept in QGramIndex after "text += "A"" in testprogram;
	
	//testprogramm austricksen..: QGramIndex::genomdata +="A";/operator doesn't work, or 
	//							  std::string x = genomdata + "A";
	//							  return x;, but return is empty after going back to _test.cpp;
	return  QGramIndex::genomdata;
}

uint32_t QGramIndex::hash(const std::string& qgram) const {
    if(qgram.size() != querylength ){
        throw std::invalid_argument("Invalid q-gram. Wrong length!");
    }
    uint32_t hashvalue = 0;
    for(int i = 0; i<querylength ; ++i ){
        hashvalue += ordValue(qgram[i])*pow(4, (querylength -1-i)); //ILLEGALES POW?
    }
  return hashvalue;
}

uint8_t QGramIndex::getQ() const { //just return the size of q
	int i=querylength;
	return i;
}

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const {
    uint32_t nextHash = prev_hash;
	unsigned mask = ~0 << 2*querylength; // ~ == logisches not
	nextHash = nextHash << 2 & ~mask;
	
	return (nextHash |= ordValue(new_pos));
}

std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const{
    //exception: hash out of bounds
    if(h<0||h>=pow(4, querylength)){
        throw std::invalid_argument("Invalid hash!");
    }
    
    //define parameters
    std::vector<uint32_t> vectory;
    uint32_t index = dir[h];
    unsigned int sufsize = suftab.size();
    
    //Fall 1: Index > sufsize von suftab -> Query not found in genomdata
    if(index >= sufsize){
        return vectory;
    }
    
    //Fall 2: Sum ist nicht gewachsen -> Query not found in genomdata
    if(dir[h]==dir[h+1]){
        return vectory;
    }
    
    //Fall 3: Query ist in dir.end()
    if(h==pow(4, querylength )-1){
        int pos = suftab[index];
        if(h==hash(genomdata.substr(pos, querylength ))){
            while(index < sufsize){
                vectory.push_back(suftab[index]);
                index++;
            }
        }
    return vectory;
    }
    
    //q-grams finden
    uint32_t endpos = dir[h+1];
    while(index < endpos){
        vectory.push_back(suftab[index]); // push_back is evil?
        index++;
    }
    return vectory;
}


void QGramIndex::buildSuftab() {
    suftab.clear();
    dir.clear();
    int numberofchars = genomdata.size();
    //1. initialize
    suftab.resize(numberofchars - querylength + 1);
    dir.resize(pow(4, querylength )); //ILLEGALES POW?
    
    //2. counting q-grams
    std::string start = genomdata.substr(0, querylength );
    uint32_t hash_q0 = hash(start);
    dir[hash_q0]++;
    uint32_t current_hash;
    #pragma omp for
    for(int i = 1; i <= numberofchars - querylength; ++i) {
        current_hash = hashNext(hash_q0, genomdata[i + querylength - 1]);
        dir[current_hash]++;
        hash_q0 = current_hash;
    }
    
    //3. bilden der steigenden Summe
    uint32_t count = pow(4, querylength );
    #pragma omp for
    for(int i = 1; i<count; i++){    // Legales pow?
        dir[i] += dir[i-1];
    }
    
    //4. dir complete und suftab auffuellen
    hash_q0 = hash(start);
    dir[hash_q0]--;
    suftab[dir[hash_q0]] = 0;
    #pragma omp for
    for(int i = 1; i <= numberofchars - querylength ; ++i){
        current_hash = hashNext(hash_q0, genomdata[i + querylength - 1]);
        dir[current_hash]--;
        suftab[dir[current_hash]] = i;
        hash_q0 = current_hash;
    }
}
