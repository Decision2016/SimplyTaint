#include "global.h"

// memory tainted list
std::hash_set<UINT64> taintedBytes;
// register tainted list
std::list<REG> regsTainted;

// malloc list
std::list<MallocArea> mallocAreaList;