#ifndef SIMPLYTAINT_GLOBAL_H
#define SIMPLYTAINT_GLOBAL_H

#include "pin.H"
#include <list>
#include <hash_set>

#define _OBSERVE

#define ALLOCATED  1
#define FREED      !ALLOCATED

typedef struct {
    UINT64 base;
    UINT64 size;
    BOOL status;
} MallocArea;

extern std::hash_set<UINT64> taintedBytes;
extern std::list<REG> regsTainted;

extern std::list<MallocArea> mallocAreaList;

#endif //SIMPLYTAINT_GLOBAL_H
