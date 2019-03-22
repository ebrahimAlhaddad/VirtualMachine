#pragma once
#include <exception>

class FatalEx: public virtual std::exception{
    
public:
    FatalEx(int inCode){
        code = inCode;
    };
    int code;
};

class NonfatalEx: public virtual std::exception{
    
public:
    NonfatalEx(int inCode){
        code = inCode;
    };
    int code;
};
