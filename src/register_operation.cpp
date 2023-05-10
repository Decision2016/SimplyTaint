#include "register_operation.h"
#include "global.h"
#include <iostream>
#include <list>

bool RegIsTainted(REG reg) {
    for (auto reg_r = regsTainted.begin(); reg_r != regsTainted.end(); reg_r++) {
        if (*reg_r == reg) {
            return true;
        }
    }
    return false;
}

bool taintReg(REG reg) {
    if (RegIsTainted(reg)) {
#ifdef _OBSERVE
        std::cout << "\n\n\n" << REG_StringShort(reg) << " is already tainted." << std::endl;
#endif

        return false;
    }

    switch (reg) {
        case REG_EAX: regsTainted.push_front(REG_EAX);
        case REG_AX: regsTainted.push_front(REG_AX);
        case REG_AH: regsTainted.push_front(REG_AH);
        case REG_AL: regsTainted.push_front(REG_AL);
            break;

        case REG_EBX: regsTainted.push_front(REG_EBX);
        case REG_BX: regsTainted.push_front(REG_BX);
        case REG_BH: regsTainted.push_front(REG_BH);
        case REG_BL: regsTainted.push_front(REG_BL);
            break;

        case REG_ECX: regsTainted.push_front(REG_ECX);
        case REG_CX: regsTainted.push_front(REG_CX);
        case REG_CH: regsTainted.push_front(REG_CH);
        case REG_CL: regsTainted.push_front(REG_CL);
            break;

        case REG_EDX: regsTainted.push_front(REG_EDX);
        case REG_DX: regsTainted.push_front(REG_DX);
        case REG_DH: regsTainted.push_front(REG_DH);
        case REG_DL: regsTainted.push_front(REG_DL);
            break;

        case REG_ESI: regsTainted.push_front(REG_ESI);
        case REG_SI: regsTainted.push_front(REG_SI);
            break;

        default:
#ifdef _OBSERVE
        std::cout << "\n\n\n" << REG_StringShort(reg) << " can't be tainted" << std::endl;
#endif
            return false;
    }

#ifdef _OBSERVE
    std::cout << "\n\n\n" << REG_StringShort(reg) << " tainted" << std::endl;
#endif
    return true;
}

bool removeRegTainted(REG reg) {
    switch(reg) {
        case REG_EAX: regsTainted.remove(REG_EAX);
        case REG_AX: regsTainted.remove(REG_AX);
        case REG_AH: regsTainted.remove(REG_AH);
        case REG_AL: regsTainted.remove(REG_AL);
            break;

        case REG_EBX: regsTainted.remove(REG_EBX);
        case REG_BX: regsTainted.remove(REG_BX);
        case REG_BH: regsTainted.remove(REG_BH);
        case REG_BL: regsTainted.remove(REG_BL);
            break;

        case REG_ECX: regsTainted.remove(REG_ECX);
        case REG_CX: regsTainted.remove(REG_CX);
        case REG_CH: regsTainted.remove(REG_CH);
        case REG_CL: regsTainted.remove(REG_CL);
            break;

        case REG_EDX: regsTainted.remove(REG_EDX);
        case REG_DX: regsTainted.remove(REG_DX);
        case REG_DH: regsTainted.remove(REG_DH);
        case REG_DL: regsTainted.remove(REG_DL);
            break;

        case REG_ESI: regsTainted.remove(REG_ESI);
        case REG_SI: regsTainted.remove(REG_SI);
            break;

        default:
            return false;
    }
#ifdef _OBSERVE
    std::cout << "\n\n\n" << REG_StringShort(reg) << " freed." << std::endl;
#endif
    return true;
}

VOID RegisterHandler(INS ins) {
    if (INS_OperandCount(ins) != 2) {
        return ;
    }

    REG reg_r = INS_RegR(ins, 0);
    REG reg_w = INS_RegW(ins, 0);

    if (REG_valid(reg_w)) {
        if (RegIsTainted(reg_w) && (!REG_valid(reg_r)) || !RegIsTainted(reg_r)) {
#ifdef _OBSERVE
            std::cout << "[SPREAD]\n\n" << INS_Address(ins) << ": " << INS_Disassemble(ins) << std::endl;
            std::cout << "\n\noutput: " << REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << std::endl;
#endif
            removeRegTainted(reg_w);
        }
        else if (!RegIsTainted(reg_w) && RegIsTainted(reg_r)) {
#ifdef _OBSERVE
            std::cout << "[SPREAD]\n\n" << INS_Address(ins) << ": " << INS_Disassemble(ins) << std::endl;
            std::cout << "\n\noutput: " << REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << std::endl;
#endif
            taintReg(reg_w);
        }
    }
}