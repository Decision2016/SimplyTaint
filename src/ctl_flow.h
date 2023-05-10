#ifndef SIMPLYTAINT_CTL_FLOW_H
#define SIMPLYTAINT_CTL_FLOW_H
#include "pin.H"

VOID CallHandler(INS ins, CONTEXT *ctx);

VOID RetHandler(INS ins, ADDRINT insAddr, std::string disAsm, UINT64 stackPtr);
#endif //SIMPLYTAINT_CTL_FLOW_H
