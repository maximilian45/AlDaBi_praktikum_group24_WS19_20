#include "Alignment.hpp"
#include <string>

int main(int argc, char* argv[]){
    
    Alignment test{"IMISSMISSISSIPPI","MYMISSISAHIPPIE"};
    //
    test.compute(3,-4,-6,false);
    
    if(argc != 5){
        
        
        
        return 0;
    }else{
        //Default Werte für match, mismatch und gap benutzen
    }
    
    
    
    return 1;
}
