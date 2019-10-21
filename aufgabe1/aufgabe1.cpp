#include "aufgabe1.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <fstream>

char complement(char const x){
    char out = 'E';    
    switch(x){
        case 'A':
            out = 'T';
            break;
        case 'T':
            out = 'A';
            break;
        case 'G':
            out = 'C';
            break;
        case 'C':
            out = 'G';
            break;
        default:
            break;
    }
    return out;
}




std::string reverseComplement(std::string const& input){
    std::string out = "";
    //for(char c : input){
    if(input==""){std::cout<<"Keine leeren Eingaben!!!";
        return out;        
        }
    for(int i = input.size()-1; i>=0;i--){
        out += complement(input[i]);        
    }

    return out;
}



std::pair<std::string, std::string> readFasta(std::string const& in_file){
    //std::cout<<"INFILE:     "<<in_file<<std::endl;
    std::string test = in_file;
    std::ifstream reader(in_file);
    std::string meta= "";
    std::string seq = "";
    std::string seq2 = "";
    if(reader.is_open()){
        getline(reader,meta,'\n');
    while(!reader.eof()){
        getline(reader,seq2,'\n');
        seq+=seq2;
    }
    
    }else{std::cout<<"Fehler beim lesen der Datei";
        reader.close();
        return std::make_pair(meta,seq);    
    }
    reader.close();
    return std::make_pair(meta,seq);

}


bool writeFasta(std::string const& out_file,
                std::string const& meta,
                std::string const& seq){
    std::string ausgabefile = out_file;
    std::ofstream writer(ausgabefile);
    if(writer.is_open()){
        writer<<meta<<'\n';
        uint32_t count = 0;
        for(uint32_t i = 0; i <seq.size();i++){
            writer<< seq[i];
            count+=1;
            if(count % 80 == 0){
                 writer<<'\n';           
            }        
        } 
    }else{std::cout<<"Fehler beim erstellen der Ausgabedatei!"<< std::endl;
        return false;}
    return true;
}


bool reverseComplementFASTA(std::string const& input_file,
                            std::string const& output_file){
    std::pair<std::string,std::string> read = readFasta(input_file);
    if(read.first == "" || read.second == ""){
        std::cout << "Datei konnte nicht gelesen werden"<<std::endl;        
        return false;}
    
    read.second = reverseComplement(read.second);
    
    return writeFasta(output_file, read.first,read.second);
}

