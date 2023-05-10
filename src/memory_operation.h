#ifndef SIMPLYTAINT_MEMORY_OPERATION_H
#define SIMPLYTAINT_MEMORY_OPERATION_H

#include "pin.H"

VOID ReadMem2RegHandler(UINT64 insAddress, std::string insDis, UINT32 opCount, REG reg_r, UINT64 memOP);

VOID WriteMemHandler(UINT64 insAddress, std::string insDis, UINT32 opCount, REG reg_r, UINT64 memOP);

VOID removeMemoryTaint(UINT64 addr);

#endif //SIMPLYTAINT_MEMORY_OPERATION_H
