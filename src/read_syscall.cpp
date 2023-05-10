#include "read_syscall.h"
#include "global.h"
#include <asm/unistd.h>
#include <iostream>

ADDRINT syscall_num = 0xffffffff;
UINT64 start = 0;
UINT fd = 0;

VOID SyscallEntryHandler(THREADID thead_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v) {
    syscall_num = PIN_GetSyscallNumber(ctx, std);

    // 判断当前的系统调用是 read
    if (syscall_num == __NR_read) {
        start = (UINT64)PIN_GetSyscallArgument(ctx, std, 1);
        fd = (UINT) PIN_GetSyscallArgument(ctx, std, 0);
    }
}

VOID SyscallExitHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v) {
    if (syscall_num == __NR_read) {
        if (fd == 3) {
            return ;
        }

        UINT64 size = PIN_GetSyscallReturn(ctx, std);
        for (UINT64 i = 0; i < size; i++) {
            taintedBytes.insert(start + 1);
        }

#ifdef _OBSERVE
        std::cout << std::hex << "[Taint] In 0x" << start << " Size: 0x" << size << " fd:" << fd << " (via read)" << std::endl;
#endif
    }

    syscall_num = 0xffffffff;
}