#include "image.h"
#include "global.h"
#include <iostream>

static size_t lastSize;

VOID callbackBeforeMalloc(ADDRINT size) {
    lastSize = size;
}

VOID callbackBeforeFree(ADDRINT address) {
#ifdef _OBSERVE
    std::cout << "[INFO] Free(" << std::hex << address << ")" << std::endl;
#endif

    for(auto area = mallocAreaList.begin(); area != mallocAreaList.end(); area ++) {
        if (address == area->base) {
            area->status = FREED;
            break;
        }
    }
}

VOID callbackAfterMalloc(ADDRINT ret) {
#ifdef _OBSERVE
    std::cout << "[INFO] Malloc(" << std::hex << ret << std::endl;
#endif

    if (ret) {
        for(auto area = mallocAreaList.begin(); area != mallocAreaList.end(); area ++) {
            if (ret == area->base) {
                area->status = ALLOCATED;
                area->size = lastSize;
                break;
            }
        }

        MallocArea  element;
        element.base = ret;
        element.size = lastSize;
        element.status = ALLOCATED;

        mallocAreaList.push_front(element);
    }
}

VOID Image(IMG img, VOID *v) {
    RTN mallocRTN = RTN_FindByName(img, "malloc");
    RTN freeRTN = RTN_FindByName(img, "free");

    if (RTN_Valid(mallocRTN)) {
        RTN_Open(mallocRTN);
        RTN_InsertCall(mallocRTN, LEVEL_VM::IPOINT_BEFORE, (AFUNPTR)callbackAfterMalloc, IARG_FUNCARG_CALLSITE_VALUE, 0, IARG_END);
        RTN_InsertCall(mallocRTN, LEVEL_VM::IPOINT_AFTER, (AFUNPTR)callbackAfterMalloc, IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);
    }

    if (RTN_Valid(freeRTN)) {
        RTN_Open(freeRTN);
        RTN_InsertCall(freeRTN, IPOINT_BEFORE, (AFUNPTR) callbackBeforeFree, IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);
        RTN_Close(freeRTN);
    }
}