#include "memory_operation.h"
#include "global.h"
#include "register_operation.h"
#include <iostream>

// 将某个地址的内存从污点列表中移除
VOID removeMemoryTaint(UINT64 address) {
    auto it = taintedBytes.find(address);

    if (it != taintedBytes.end()) {
        taintedBytes.erase(it);

#ifdef _OBSERVE
        std::cout << std::hex << "\n\n\n" << address << "freed now." << std::endl;
#endif
    }
}

// 在污点信息列表中添加新的内存地址
VOID insertMemoryTaint(UINT64 address) {
    taintedBytes.insert(address);
#ifdef _OBSERVE
    std::cout << std::hex << "\n\n\n" << address << "tainted now." << std::endl;
#endif
}

VOID ReadMem2RegHandler(UINT64 insAddress, std::string insDis, UINT32 opCount, REG reg_r, UINT64 memOP) {
    UINT64 address = memOP;

    if (opCount != 2) {
        return;
    }

    for (auto area = mallocAreaList.begin(); area != mallocAreaList.end(); area++) {
        // 如果使用到了一个已经被释放的区域， 输出 UAF 信息
        if (address >= area->base && address < (area->base + area->size) && area->status == FREED) {
            std::cout << std::hex << "[UAF in " << address << "]\n" << insAddress << ": " << insDis << std::endl;
            return;
        }
    }

    // 如果 read 函数作用的内存区域是被标记污点的， 同样给寄存器标记污点
    auto it = taintedBytes.find(address);
    if (it != taintedBytes.end()) {
        taintReg(reg_r);
#ifdef _OBSERVE
        std::cout << std::hex << "[READ in " << address << "]\n" << insAddress << ": " << insDis << std::endl;
#endif
        return;
    }

    // 否则，说明内存区域没有被标记， 将这个寄存器的标记移除
    if (RegIsTainted(reg_r)) {
#ifdef _OBSERVE
        removeRegTainted(reg_r);
#endif
    }
}

VOID WriteMemHandler(UINT64 insAddress, std::string insDis, UINT32 opCount, REG reg_r, UINT64 memOP) {
    UINT64 address = memOP;

    if (opCount != 2) {
        return;
    }

    // 类似地，进行 UAF 检查
    for (auto area = mallocAreaList.begin(); area != mallocAreaList.end(); area++) {
        if (address >= area->base && address < (area->base + area->size) && area->status == FREED) {
            std::cout << std::hex << "[UAF in " << address << "]\n" << insAddress << ": " << insDis << std::endl;
            return;
        }
    }

    // 如果寄存器没有被标记， 移除当前内存区域的标记
    auto it = taintedBytes.find(address);
    if (it != taintedBytes.end()) {
        if (!REG_valid(reg_r) || RegIsTainted(reg_r)) {
            removeMemoryTaint(address);
        }
#ifdef _OBSERVE
        std::cout << std::hex << "[WRITE in " << address << "]\n" << insAddress << ": " << insDis << std::endl;
#endif
        return;
    }

    // 如果寄存器被标记了， 那么同样地标记内存
    if (RegIsTainted(reg_r)) {
#ifdef _OBSERVE
        std::cout << std::hex << "[WRITE in " << address << "]\n" << insAddress << ": " << insDis << std::endl;
#endif
        insertMemoryTaint(address);
    }
}