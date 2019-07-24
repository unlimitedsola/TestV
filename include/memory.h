#ifndef TESTV_MEMORY_H
#define TESTV_MEMORY_H

#include <Windows.h>
#include <TlHelp32.h>

struct Module {
    DWORD64 dwBase, dwSize;
};

Module TargetModule;
HANDLE TargetProcess;
DWORD TargetId;

HANDLE GetProcess(const wchar_t *processName) {
    HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof entry;

    do
        if (!_wcsicmp(entry.szExeFile, processName)) {
            TargetId = entry.th32ProcessID;
            CloseHandle(handle);
            TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, false, TargetId);
            return TargetProcess;
        }
    while (Process32Next(handle, &entry));

    return nullptr;
}

Module GetModule(const wchar_t *moduleName) {
    HANDLE hmodule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, TargetId);
    MODULEENTRY32 mEntry;
    mEntry.dwSize = sizeof mEntry;

    do {
        if (!_wcsicmp(mEntry.szModule, moduleName)) {
            CloseHandle(hmodule);

            TargetModule = {DWORD64(mEntry.hModule), mEntry.modBaseSize};
            return TargetModule;
        }
    } while (Module32Next(hmodule, &mEntry));

    Module mod = {(DWORD64) false, (DWORD64) false};
    return mod;
}

template<typename var>
BOOL WriteMemory(DWORD64 Address, var Value) {
    return WriteProcessMemory(TargetProcess, (LPVOID) Address, &Value, sizeof(var), nullptr);
}

template<typename var>
var ReadMemory(DWORD64 Address) {
    var value;
    ReadProcessMemory(TargetProcess, (LPCVOID) Address, &value, sizeof(var), nullptr);
    return value;
}

BOOL MemoryCompare(const BYTE *bData, const BYTE *bMask, const char *szMask) {
    for (int k = 0; *szMask; ++szMask, ++bData, ++bMask, ++k) {
        if (static_cast<char>((k ^ 1) ? (~(*szMask)) : ((*szMask) ^ 0xDA)) == 'x'
            && *bData != static_cast<uint8_t>((k ^ 1) ? ((*bMask) ^ 0xDA) : (~(*bMask)))) {
            return false;
        }
    }
    return *szMask == NULL;
}

uintptr_t FindPointer(uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;
    for (unsigned int offset : offsets) {
        ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(addr), &addr, sizeof addr, nullptr);
        addr += offset;
    }
    return addr;
}

DWORD64 FindSignature(DWORD64 start, DWORD64 size, const char *sig, const char *mask) {
    auto *data = new BYTE[size];
    SIZE_T bytesRead;

    ReadProcessMemory(TargetProcess, (LPVOID) start, data, size, &bytesRead);

    for (DWORD64 i = 0; i < size; i++) {
        if (MemoryCompare((const BYTE *) (data + i), (const BYTE *) sig, mask)) {
            return start + i;
        }
    }
    delete[] data;
    return NULL;
}

DWORD64 FindSignature(const char *sig, const char *mask) {
    return FindSignature(TargetModule.dwBase, TargetModule.dwSize, sig, mask);
}

#endif //TESTV_MEMORY_H
