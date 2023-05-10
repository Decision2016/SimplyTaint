#include "instruction.h"
#include "memory_operation.h"
#include "register_operation.h"
#include "ctl_flow.h"

#include <iostream>
#include <asm/unistd.h>

VOID Instruction(INS ins, VOID *v) {
    // 插入 read 调用的 handler， 例如 move ax, [bp + 0x30]
    if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 0)) {
        INS_InsertCall(ins, LEVEL_VM::IPOINT_BEFORE, (AFUNPTR) ReadMem2RegHandler, IARG_ADDRINT,
                       INS_Address(ins), IARG_PTR, new std::string(INS_Disassemble(ins)), IARG_UINT32, INS_OperandCount(ins),
                       IARG_UINT32, INS_OperandReg(ins, 0), IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    // 插入 write 调用的 handler， 例如 move word prt [di], dx
    } else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WriteMemHandler, IARG_ADDRINT, INS_Address(ins), IARG_PTR,
                       new std::string(INS_Disassemble(ins)), IARG_UINT32, INS_OperandCount(ins), IARG_UINT32, INS_OperandReg(ins, 1),
                       IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    // 插入寄存器之间进行 mov 的 handler，例如 mov ax, dx
    } else if(INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegisterHandler, IARG_PTR, ins, IARG_END);
    // 插入 RETURN 的 handler
    } else if(INS_IsRet(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RetHandler, IARG_PTR, ins, IARG_ADDRINT, INS_Address(ins),
                       IARG_PTR, new std::string(INS_Disassemble(ins)), IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    // 插入 CALL 的 handler
    } else if(INS_IsCall(ins)) {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)CallHandler, IARG_PTR, ins, IARG_CONTEXT, IARG_END);
    }
}
