#include "Machine.h"

Machine::Machine():mImage(320,240),mDrawer(mImage)
{
    
    //clear image
    mImage.set_all_channels(0,0,0);
    //set up drawer pen attributes
    mDrawer.pen_width(1);
    mDrawer.pen_color(255, 255, 255);
	// TODO
    //add operations to the Machine's map
    opMap.emplace("movi",&CreateOp<MovI>);
    opMap.emplace("exit",&CreateOp<Exit>);
    opMap.emplace("add",&CreateOp<Add>);
    opMap.emplace("mov",&CreateOp<Mov>);
    opMap.emplace("sub",&CreateOp<Sub>);
    opMap.emplace("mul",&CreateOp<Mul>);
    opMap.emplace("div",&CreateOp<Div>);
    opMap.emplace("inc",&CreateOp<Inc>);
    opMap.emplace("dec",&CreateOp<Dec>);
    opMap.emplace("jmp",&CreateOp<Jmp>);
    opMap.emplace("jt",&CreateOp<Jt>);
    opMap.emplace("cmpeq",&CreateOp<Cmpeq>);
    opMap.emplace("cmplt",&CreateOp<Cmplt>);
    opMap.emplace("push",&CreateOp<Push>);
    opMap.emplace("pop",&CreateOp<Pop>);
    opMap.emplace("load",&CreateOp<Load>);
    opMap.emplace("store",&CreateOp<Store>);
    opMap.emplace("loadi",&CreateOp<Loadi>);
    opMap.emplace("storei",&CreateOp<Storei>);
    opMap.emplace("loadsc",&CreateOp<Loadsc>);
    opMap.emplace("storesc",&CreateOp<Storesc>);
    opMap.emplace("pendown",&CreateOp<Pendown>);
    opMap.emplace("penup",&CreateOp<Penup>);
    opMap.emplace("fwd",&CreateOp<Fwd>);
    opMap.emplace("back",&CreateOp<Back>);
    
    //setup registers map
    memRegs.emplace("r0",0);
    memRegs.emplace("r1",0);
    memRegs.emplace("r2",0);
    memRegs.emplace("r3",0);
    memRegs.emplace("r4",0);
    memRegs.emplace("r5",0);
    memRegs.emplace("r6",0);
    memRegs.emplace("r7",0);
    memRegs.emplace("pc",0);
    memRegs.emplace("sc",0);
    memRegs.emplace("ex",0);
    memRegs.emplace("tx",0);
    memRegs.emplace("ty",0);
    memRegs.emplace("tr",0);
    memRegs.emplace("tc",0);
    
    //setup flags register
    memFlags.emplace("exit",false);
    memFlags.emplace("pen",false);
    memFlags.emplace("test",false);
}

//readFile: access instruction file and update program memory
//Input: instruction file directory
void Machine::readFile(const std::string& fileDir){
    std::ifstream iFile(fileDir);
    if(iFile.is_open()){
        std::string opName;
        std::string params;
        std::string line;
        while(!iFile.eof()){
            //seperate op name and its parameters
            std::getline(iFile,line);
            short spaceInd = line.find(" ");
            if(spaceInd == -1){
                opName = line;
                params = "";
            } else {
                opName = line.substr(0,spaceInd);
                params = line.substr(spaceInd+1);
            }
            //identify operation class
            std::shared_ptr<Op> ptr = opMap.at(opName)();
            //parse operation parameters
            ptr->Parse(params);
            //add operation to the op stack
            mOps.push_back(ptr);
        }
    }
    iFile.close();
}

//
void Machine::printMemory(std::ofstream& filestream){
    filestream << "********************" << std::endl;
    filestream << "r0=" << memRegs.at("r0") <<std::endl;
    filestream << "r1=" << memRegs.at("r1") <<std::endl;
    filestream << "r2=" << memRegs.at("r2") <<std::endl;
    filestream << "r3=" << memRegs.at("r3") <<std::endl;
    filestream << "r4=" << memRegs.at("r4") <<std::endl;
    filestream << "r5=" << memRegs.at("r5") <<std::endl;
    filestream << "r6=" << memRegs.at("r6") <<std::endl;
    filestream << "r7=" << memRegs.at("r7") <<std::endl;
    filestream << "pc=" << memRegs.at("pc") <<std::endl;
    filestream << "sc=" << memRegs.at("sc") <<std::endl;
    filestream << "ex=" << memRegs.at("ex") <<std::endl;
    filestream << "tx=" << memRegs.at("tx") <<std::endl;
    filestream << "ty=" << memRegs.at("ty") <<std::endl;
    filestream << "tr=" << memRegs.at("tr") <<std::endl;
    filestream << "tc=" << memRegs.at("tc") <<std::endl;
    filestream << "exit=" << memFlags.at("exit") <<std::endl;
    filestream << "test=" << memFlags.at("test") <<std::endl;
    filestream << "pen=" << memFlags.at("pen") <<std::endl;
}

void Machine::execute() {
    
    
    //execute program operations
    std::ofstream oFile("log.txt");
    std::ofstream oFile2("stack.txt");
    //print inital memory status
    printMemory(oFile);
    printStack(oFile2);
    while(!memFlags.at("exit")){
        //increment counter and execute command
        int oldpc = memRegs.at("pc");
        auto ptr = mOps[oldpc];
        ++memRegs.at("pc");
        
        //execute operations while checking for exceptions
        try{
        ptr->Execute(*this);
        } catch(FatalEx& e){
            memRegs.at("ex") = e.code;
            memFlags.at("exit") = true;
        } catch(NonfatalEx& e){
            memRegs.at("ex") = e.code;
        }
        
        //print execution info
        oFile << "Executing: " << ptr->GetName() << std::endl;
        printMemory(oFile);
        printStack(oFile2);
    }
    //save image
    mImage.save_image("output.bmp");
    oFile.close();
        
    
    
}

//setter/getter functions 
int Machine::getReg(std::string regName) const {
    return memRegs.at(regName);
}

bool Machine::getFlag(std::string flagName) const{
    return memFlags.at(flagName);
}

void Machine::setReg(std::string regName, int val){
    if(regName == "sc"){
        throw NonfatalEx(10);
    }
    else if(regName == "r0"){
        throw NonfatalEx(11);
    }else if(regName == "tc" && (val < 0 || val > 7)){
        throw NonfatalEx(14);
    }else{
        memRegs.at(regName) = val;
    }
}

void Machine::setFlag(std::string flagName, bool val){
    memFlags.at(flagName) = val;
}

//memory stack push, pop and access functions
void Machine::push_stack( std::string input){
    
    if(memRegs.at("sc") >= 256){
        throw FatalEx(101);
    } else {
    int value = memRegs.at(input);
    stack.push_back(value);
    ++memRegs.at("sc");
    }
}

int Machine::pop_stack(){
    if(memRegs.at("sc") == 0){
        throw FatalEx(103);
    }else{
    --memRegs.at("sc");
    int value = stack.back();
    stack.pop_back();
    return value;
    }
}

int Machine::stack_get(std::string indReg){
    int ind = memRegs.at(indReg);
    return stack[ind];
}

void Machine::stack_set(std::string indReg,int newVal){
    int ind = memRegs.at(indReg);
    if(ind < 0 || ind >= memRegs.at("sc")){
        throw FatalEx(100);
    } else {
    stack[ind] = newVal;
    }
}

//stack print out function
void Machine::printStack(std::ofstream& filestream){
    filestream << "pc: " << getReg("pc") << " Stack: ";
    for(int elem:stack){
        filestream << elem << " ";
    }
    filestream << std::endl;
}

//turtle move forward function
void Machine::moveForward(std::string reg1){
    int tx = memRegs.at("tx");
    int ty = memRegs.at("ty");
    float angle = memRegs.at("tr") * 0.0174533f;
    int endx = tx + static_cast<int>(std::cos(angle) * memRegs.at(reg1));
    int endy = ty + static_cast<int>(std::sin(angle) * memRegs.at(reg1));
    if(memFlags.at("pen")){
        penColor();
        mDrawer.line_segment(tx, ty, endx, endy);
    }
    memRegs.at("tx") = endx;
    memRegs.at("ty") = endy;
}
//turtle move backward function
void Machine::moveBackward(std::string reg1){
    int tx = memRegs.at("tx");
    int ty = memRegs.at("ty");
    float angle = memRegs.at("tr") * 0.0174533f;
    int endx = tx + static_cast<int>(std::cos(angle) * (-1 * memRegs.at(reg1)));
    int endy = ty + static_cast<int>(std::sin(angle) * (-1 * memRegs.at(reg1)));
    if(memFlags.at("pen")){
        penColor();
        mDrawer.line_segment(tx, ty, endx, endy);
    }
    memRegs.at("tx") = endx;
    memRegs.at("ty") = endy;
}

//pen color change function
void Machine::penColor(){
    int tcVal = memRegs.at("tc");
    switch(tcVal){
        case 0:{
            mDrawer.pen_color(255, 255, 255);
            break;
        }
        case 1:{
            mDrawer.pen_color(255, 255,0);
            break;
        }
        case 2:{
            mDrawer.pen_color(255, 0, 255);
            break;
        }
        case 3:{
            mDrawer.pen_color(255, 0,0);
            break;
        }
        case 4:{
            mDrawer.pen_color(0, 255, 255);
            break;
        }
        case 5:{
            mDrawer.pen_color(0, 255,0);
            break;
        }
        case 6:{
            mDrawer.pen_color(0, 0, 255);
            break;
        }
        case 7:{
            mDrawer.pen_color(0,0,0);
            break;
        }
    }
}
