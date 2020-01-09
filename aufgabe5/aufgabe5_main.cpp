#include <iostream>
#include <fstream>
#include <string>
#include "QGramIndex.hpp"
#include "a5_util.hpp"
#include <omp.h>

int main(int argc, char* argv[]) {
    // read text from in_file
    
    std::string text;
    std::ifstream data (argv[1]);
    if (!data.good()){
		    std::cerr << "Unable to open requested file!" << std::endl;
		    return 1; //cancel
    } else {
        getline(data, text); //T from file to text
    }

    // Get pattern and length of it
    std::string query = argv[2];
    uint8_t q = query.length();
    
    // Generate Q as instance from class with given parameters
    int threads = atoi(argv[3]);
    // int start = omp_get_wtime();
    omp_set_num_threads(threads);
    QGramIndex Q(text,q);
    // std::cout << "ENDZEIT" << omp_get_wtime()-start <<std::endl;
    
    // Get the hits and take its vectors' size
    std::vector<uint32_t> hits = Q.getHits(Q.hash(query));
    int hitweight = hits.size();
    
    // formatting the output-console
    std::cout << query << ":";
    if(hits.empty()) {
      std::cout << " No hits found!" << std::endl;
    } else {
        for(int i=0; i<hitweight; ++i){ //for number of hits print them and seperate by space
            std::cout << " " << hits[i];
        }
    std::cout << std::endl; //not even necessary
    }
    return 0; ///done
}
