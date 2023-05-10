#ifndef SIMPLYTAINT_REGISTER_OPERATION_H
#define SIMPLYTAINT_REGISTER_OPERATION_H

#include "pin.H"

VOID RegisterHandler(INS ins);

bool RegIsTainted(REG reg);

bool taintReg(REG reg);

bool removeRegTainted(REG reg);

#endif //SIMPLYTAINT_REGISTER_OPERATION_H
