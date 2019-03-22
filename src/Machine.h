#pragma once
#include "Exceptions.h"
#include "Ops.h"
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <iostream>
#include "../img/bitmap_image.hpp"
class Machine
{
public:
    void readFile(const std::string& fileDir);
    void execute();
	Machine();
    std::map<std::string, std::function<std::shared_ptr<Op>()>> opMap;
    bool getFlag(std::string flagName) const;
    int getReg(std::string regName) const;
    void setFlag(std::string flagName, bool val);
    void setReg(std::string regName, int val);
    void push_stack( std::string);
    int pop_stack();
    int stack_get( std::string indReg);
    void stack_set( std::string indReg,int newVal);
    std::vector<int> stack;
    void moveForward(std::string reg1);
    void moveBackward(std::string reg1);
    private:
    std::map<std::string, int> memRegs;
    std::map<std::string, bool> memFlags;
	std::vector<std::shared_ptr<Op>> mOps;
    void printMemory(std::ofstream& filestream);
    void printStack(std::ofstream& filestream);
    bitmap_image mImage;
    image_drawer mDrawer;
    void penColor();
};

template <typename T>
std::shared_ptr<Op> CreateOp(){
    return std::make_shared<T>();
    }
