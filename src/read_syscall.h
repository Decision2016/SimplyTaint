#ifndef SIMPLYTAINT_READ_SYSCALL_H
#define SIMPLYTAINT_READ_SYSCALL_H

#include "pin.H"

VOID SyscallEntryHandler(THREADID thead_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);

VOID SyscallExitHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);

#endif //SIMPLYTAINT_READ_SYSCALL_H
