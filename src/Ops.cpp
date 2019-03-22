#include "Ops.h"
#include "Machine.h"

void Exit::Execute(Machine& machine)
{
	// TODO
    machine.setFlag("exit", true);
}

void MovI::Execute(Machine& machine)
{
	// TODO
    machine.setReg(std::get<0>(mParameters), std::get<1>(mParameters));
}
void Mov::Execute(Machine& machine){
    machine.setReg(std::get<0>(mParameters), machine.getReg(std::get<1>(mParameters)));
}

void Add::Execute(Machine& machine){
    int64_t firstVal = static_cast<int64_t>(machine.getReg(std::get<1>(mParameters)));
    int64_t secondVal = static_cast<int64_t>(machine.getReg(std::get<2>(mParameters)));
    secondVal += firstVal;
    machine.setReg(std::get<0>(mParameters), secondVal);
    if(secondVal > INT32_MAX){
        throw NonfatalEx(12);
    }else if(secondVal < INT32_MIN){
        throw NonfatalEx(13);
    }

    
}


void Sub::Execute(Machine& machine){
    int64_t firstVal = static_cast<int64_t>(machine.getReg(std::get<1>(mParameters)));
    int64_t secondVal = static_cast<int64_t>(machine.getReg(std::get<2>(mParameters)));
    secondVal = firstVal - secondVal;
    machine.setReg(std::get<0>(mParameters), secondVal);
    if(secondVal > INT32_MAX){
        throw NonfatalEx(12);
    }else if(secondVal < INT32_MIN){
        throw NonfatalEx(13);
    }
   
        
    
}

void Mul::Execute(Machine& machine){
    int64_t firstVal = static_cast<int64_t>(machine.getReg(std::get<1>(mParameters)));
    int64_t secondVal = static_cast<int64_t>(machine.getReg(std::get<2>(mParameters)));
    secondVal *= firstVal;
    machine.setReg(std::get<0>(mParameters), secondVal);
    if(secondVal > INT32_MAX){
        throw NonfatalEx(12);
    }else if(secondVal < INT32_MIN){
        throw NonfatalEx(13);
    }
    
        
   
}

void Div::Execute(Machine& machine){
    int64_t firstVal = static_cast<int64_t>(machine.getReg(std::get<1>(mParameters)));
    int64_t secondVal = static_cast<int64_t>(machine.getReg(std::get<2>(mParameters)));
    if(secondVal == 0){
        throw FatalEx(102);
    }else {
        secondVal = firstVal / secondVal;
        machine.setReg(std::get<0>(mParameters), secondVal);
        if(secondVal > INT32_MAX){
            throw NonfatalEx(12);
        }else if(secondVal < INT32_MIN){
            throw NonfatalEx(13);
        }
       
    }
    
    
}

void Inc::Execute(Machine& machine){
    int64_t value = static_cast<int64_t>(machine.getReg(std::get<0>(mParameters)));
    ++value;
    machine.setReg(std::get<0>(mParameters), value);
    if(value > INT32_MAX){
        throw NonfatalEx(12);
    }else if(value < INT32_MIN){
        throw NonfatalEx(13);
    }
    
}

void Dec::Execute(Machine& machine){
    int64_t value = static_cast<int64_t>(machine.getReg(std::get<0>(mParameters)));
    --value;
    machine.setReg(std::get<0>(mParameters), value);
    if(value > INT32_MAX){
        throw NonfatalEx(12);
    }else if(value < INT32_MIN){
        throw NonfatalEx(13);
    }
    
}

void Jmp::Execute(Machine& machine){
    machine.setReg("pc", machine.getReg(std::get<0>(mParameters)));
    
}

void Cmpeq::Execute(Machine& machine){
    int val1 = machine.getReg(std::get<0>(mParameters));
    int val2 = machine.getReg(std::get<1>(mParameters));
    machine.setFlag("test", val1 == val2);
}

void Cmplt::Execute(Machine& machine){
    int val1 = machine.getReg(std::get<0>(mParameters));
    int val2 = machine.getReg(std::get<1>(mParameters));
    machine.setFlag("test", val1 < val2);
}

void Jt::Execute(Machine& machine){
    if(machine.getFlag("test")){
        machine.setReg("pc", machine.getReg(std::get<0>(mParameters)));
    }
}

void Push::Execute(Machine& machine){
    machine.push_stack(std::get<0>(mParameters));
}

void Pop::Execute(Machine& machine){
    machine.setReg(std::get<0>(mParameters), machine.pop_stack());
}

void Load::Execute(Machine& machine){
    machine.setReg(std::get<0>(mParameters), machine.stack_get(std::get<1>(mParameters)));
}

void Store::Execute(Machine& machine){
    machine.stack_set(std::get<0>(mParameters), machine.getReg(std::get<1>(mParameters)));
}

void Loadi::Execute(Machine& machine){
    int stackInd = std::get<1>(mParameters);
    if(stackInd < 0 || stackInd >= machine.getReg("sc")){
        throw FatalEx(100);
    } else {
    machine.setReg(std::get<0>(mParameters), machine.stack[stackInd]);
    }
}

void Storei::Execute(Machine& machine){
    machine.stack[std::get<0>(mParameters)] = machine.getReg(std::get<1>(mParameters));
}

void Loadsc::Execute(Machine& machine){
    int reg2Val = machine.getReg(std::get<1>(mParameters));
    int scVal = machine.getReg("sc");
    int stackInd = scVal-reg2Val-1;
    if(stackInd < 0 || stackInd >= machine.getReg("sc")){
        throw FatalEx(100);
    } else {
    machine.setReg(std::get<0>(mParameters), machine.stack[stackInd]);
    }
}

void Storesc::Execute(Machine& machine){
    int reg1Val = machine.getReg(std::get<0>(mParameters));
    int scVal = machine.getReg("sc");
    int reg2Val = machine.getReg(std::get<1>(mParameters));
    int stackInd = scVal-reg1Val-1;
    if(stackInd < 0 || stackInd >= machine.getReg("sc")){
        throw FatalEx(100);
    } else {
    machine.stack[stackInd] = reg2Val;
    }
}

void Pendown::Execute(Machine& machine){
    machine.setFlag("pen", true);
}

void Penup::Execute(Machine& machine){
    machine.setFlag("pen", false);
}

void Fwd::Execute(Machine& machine){
    machine.moveForward(std::get<0>(mParameters));
}

void Back::Execute(Machine& machine){
    machine.moveBackward(std::get<0>(mParameters));
}
