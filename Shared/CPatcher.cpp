//============== Networked: IV - http://code.networked-iv.com ==============
//
// File: CPatcher.cpp
// Project: Shared
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==========================================================================


#include <StdInc.h>


ProtectionInfo CPatcher::Unprotect(DWORD dwAddress, int iSize)
{
        ProtectionInfo protectionInfo;
        protectionInfo.dwAddress = dwAddress;
        protectionInfo.iSize = iSize;
        VirtualProtect((void *)dwAddress, iSize, PAGE_EXECUTE_READWRITE, &protectionInfo.dwOldProtection);
        return protectionInfo;
}


void CPatcher::Reprotect(ProtectionInfo protectionInfo)
{
        DWORD dwProtection;
        VirtualProtect((void *)protectionInfo.dwAddress, protectionInfo.iSize, protectionInfo.dwOldProtection, &dwProtection);
}


void CPatcher::InstallNopPatch(DWORD dwAddress, int iSize)
{
        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect(dwAddress, iSize);


        // Write the no operation to the address memory
        memset((void *)dwAddress, X86_NOP, iSize);


        // Re-protect the address memory
        Reprotect(protectionInfo);
}


void * CPatcher::InstallDetourPatchInternal(DWORD dwAddress, DWORD dwDetourAddress, BYTE byteType, int iSize)
{
        BYTE * pbyteAddr = (BYTE *)dwAddress;


        // Allocate the trampoline memory
        BYTE * pbyteTrampoline = (BYTE *)malloc(iSize + 5);


        // Unprotect the trampoline memory
        Unprotect((DWORD)pbyteTrampoline, (iSize + 5));


        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect((DWORD)pbyteAddr, (iSize + 5));


        // Copy the overwritten address memory to the trampoline memory
        memcpy(pbyteTrampoline, pbyteAddr, iSize);


        // Write the type to the trampoline memory
        pbyteTrampoline[iSize] = byteType;


        // Write the detour to the trampoline memory
        *(void **)(&pbyteTrampoline[iSize + 1]) = (void *)((pbyteAddr + iSize) - (pbyteTrampoline + iSize) - 5);


        // Write the type to the address memory
        pbyteAddr[0] = byteType;


        // Write the detour to the address memory
        *(void **)(&pbyteAddr[1]) = (void *)((BYTE *)dwDetourAddress - pbyteAddr - 5);


        // Re-protect the address memory
        Reprotect(protectionInfo);


        return pbyteTrampoline;
}


void CPatcher::UninstallDetourPatchInternal(DWORD dwAddress, void ** pTrampoline, int iSize)
{
        BYTE * pTramp = (BYTE *)*pTrampoline;
        BYTE * pbyteAddr = (BYTE *)dwAddress;


        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect((DWORD)pbyteAddr, iSize);


        // Copy the trampoline to the address
        memcpy(pbyteAddr, pTramp, iSize);


        // Re-protect the address memory
        Reprotect(protectionInfo);


        // Free trampoline
        free(pTramp);


        // Set trampoline to the address
        *pTrampoline = pbyteAddr;
}


void * CPatcher::InstallJmpPatch(DWORD dwAddress, DWORD dwJmpAddress, int iSize)
{
        return InstallDetourPatchInternal(dwAddress, dwJmpAddress, X86_JMP, iSize);
}


void * CPatcher::InstallCallPatch(DWORD dwAddress, DWORD dwCallAddress, int iSize)
{
        return InstallDetourPatchInternal(dwAddress, dwCallAddress, X86_CALL, iSize);
}


void CPatcher::InstallRetnPatch(DWORD dwAddress)
{
        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect(dwAddress, 1);


        // Write the return to the address memory
        *(BYTE *)dwAddress = X86_RETN;


        // Re-protect the address memory
        Reprotect(protectionInfo);
}


void CPatcher::InstallStringPatch(DWORD dwAddress, char * szString, int iSize)
{
        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect(dwAddress, iSize);


        // Write the string to the address memory
        memcpy((void *)dwAddress, szString, iSize);


        // Re-protect the address memory
        Reprotect(protectionInfo);
}


void CPatcher::InstallMethodPatch(DWORD dwAddress, DWORD dwFunctionAddress)
{
        // Unprotect the address memory
        ProtectionInfo protectionInfo = Unprotect(dwAddress, 4);


        // Write the function to the address memory
        *(DWORD *)dwAddress = (DWORD)dwFunctionAddress;


        // Re-protect the address memory
        Reprotect(protectionInfo);
}


DWORD CPatcher::GetFunctionAddress(char * szLibrary, char * szFunction)
{
        return (DWORD)GetProcAddress(GetModuleHandle(szLibrary), szFunction);
}


DWORD CPatcher::GetFunctionAddress(char * szLibrary, unsigned int uOrdinal)
{
        return GetFunctionAddress(szLibrary, (char *)MAKELONG(uOrdinal, 0));
}


void * CPatcher::InstallDetourPatch(char * szLibrary, char * szFunction, DWORD dwFunctionAddress)
{
        return DetourFunction(DetourFindFunction(szLibrary, szFunction), (BYTE *)dwFunctionAddress);
        //return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, szFunction), dwFunctionAddress, X86_JMP, 5, false);
}


void * CPatcher::InstallDetourPatch(char * szLibrary, unsigned int uOrdinal, DWORD dwFunctionAddress)
{
        return DetourFunction(DetourFindFunction(szLibrary, (char *)MAKELONG(uOrdinal, 0)), (BYTE *)dwFunctionAddress);
        //return InstallDetourPatchInternal(GetFunctionAddress(szLibrary, uOrdinal), dwFunctionAddress, X86_JMP, 5, false);
}


bool CPatcher::UninstallDetourPatch(void * pTrampoline, DWORD dwFunctionAddress)
{
        return (DetourRemove((BYTE *)pTrampoline, (BYTE *)dwFunctionAddress) != 0);
}
