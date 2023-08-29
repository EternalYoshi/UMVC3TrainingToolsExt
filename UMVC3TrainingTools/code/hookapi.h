#pragma once


#ifndef HOOKAPI_H
#define HOOKAPI_H

#include <Windows.h>
#include <stdint.h>

#define HOOK_READEX(proc, handle, var) ReadProcessMemory(proc, (void*) (handle), &var, sizeof(var), NULL)
#define HOOK_WRITEEX(proc, handle, var) WriteProcessMemory(proc, (void*) (handle), &var, sizeof(var), NULL)

#ifdef __cplusplus
extern "C" {
#endif

    typedef uint64_t hook_handle64_t;

    typedef struct _hook_entry64 {
        // these must be initialized before being passed into the function
        HANDLE proc;          // process to hook in
        uint64_t addr;        // the address to hook
        uint32_t replaceSize; // how many bytes at the hook address to overwrite to make room for our hook

        uint8_t* originalCode;
        uint8_t* redirectCode;
        hook_handle64_t hookData;
        hook_handle64_t remoteCode;
    } hook_entry64_t;

    /**
     * Injects shellcode into an external x64 process. The region [addr->(addr+replaceSize)]
     * must not contain any relative jmp/calls and must not use the RDX register. Also, the
     * replaceSize must be at least 14 and be instruction aligned. You could lower the
     * replaceSize to 12 bytes if you do not care about the contents of RDX.
     *
     * You can create the shellcode however you like, but I used the online assembler here:
     * https://defuse.ca/online-x86-assembler.htm
     *
     * @param ent the structure vars proc, addr, and replaceSize must be initialized by the caller
     * @param shellcode
     * @param shellcodeSize
     * @param hookDataSize how much memory to reserve for use by shellcode in the external process
     * @param prepShellcodeCallback a callback function to modify the shellcode with the address of the allocated data region
     * @return 1 on success, 0 on fail (note: the hook_entry64_t doesn't have to be freed if this fails)
     */
    int hook_install_x64(hook_entry64_t* ent, uint8_t* shellcode, uint32_t shellcodeSize, uint32_t hookDataSize, void (*prepShellcodeCallback)(uint64_t remoteData, uint8_t* injectedAsm));

    /**
     * Uninstalls a hook
     *
     * @param ent
     * @return 1 on success, 0 on failure
     */
    int hook_uninstall(hook_entry64_t* ent);

    /**
     * If you are not going to be uninstalling your hook later,
     * then you need to free the hook entry.
     *
     * @param ent
     */
    void hook_free(hook_entry64_t* ent);

#ifdef __cplusplus
}
#endif

#endif //HOOKAPI_H

