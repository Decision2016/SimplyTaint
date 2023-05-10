#include "ctl_flow.h"
#include "global.h"
#include "memory_operation.h"
#include <iostream>

VOID CallHandler(INS ins, CONTEXT *ctx) {
    ADDRINT sp = PIN_GetContextReg(ctx, LEVEL_BASE::REG_SP);

    removeMemoryTaint(sp - 8);
}

VOID RetHandler(INS ins, ADDRINT insAddr, std::string disAsm, UINT64 stackPtr) {
    auto it = taintedBytes.find(stackPtr);
    if (it != taintedBytes.end()) {
        std::cout << "\033[1;31mOverflow occured!\033[0m" << std::endl;
        std::cout << std::hex << "Position: 0x" << insAddr << "\nInstruction: " << disAsm << "\nSP: 0x" << stackPtr << std::endl;
        exit(0);
    }
}