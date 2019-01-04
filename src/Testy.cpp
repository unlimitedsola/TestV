#include "vector.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <vector>

using namespace std;

DWORD64 WorldPTR;
DWORD64 BlipPTR;
DWORD64 PlayerPTR;
DWORD64 GlobalPTR;

#pragma region usefullstuff

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

using std::cout;
using std::endl;
using std::string;

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

#pragma endregion

uintptr_t WorldPtrBaseAddr;
uintptr_t BlipPtrBaseAddr;
uintptr_t AfkPtrBaseAddr;

std::vector<unsigned int> OffsetsAFK = {0x2C0};
std::vector<unsigned int> OffsetsWantedLevel = {0x8, 0x10B8, 0x818};
std::vector<unsigned int> OffsetsGodMode = {0x8, 0x280};
std::vector<unsigned int> OffsetsGodModetwo = {0x8, 0x189};
std::vector<unsigned int> OffsetsRagdoll = {0x8, 0x10A8};
std::vector<unsigned int> OffsetPlayerNav = {0x8, 0x30, 0x50};
std::vector<unsigned int> OffsetPlayerPos = {0x8, 0x90};
std::vector<unsigned int> OffsetIsInVehicle = {0x8, 0x146B};
std::vector<unsigned int> OffsetCarPos = {0x8, 0xD28, 0x30, 0x50};
std::vector<unsigned int> OffsetCarNav = {0x8, 0xD28, 0x30, 0x90};
std::vector<unsigned int> OffsetsCarGodMode = {0x8, 0xD28, 0x189};

void teleport(v3 vec) {
    const uintptr_t is_in_vehicle_address = FindPointer(WorldPtrBaseAddr, OffsetIsInVehicle);
    const BOOL is_in_vehicle = ReadMemory<BYTE>(is_in_vehicle_address) == 0;
    if (GetAsyncKeyState(VK_SHIFT) & (1 << 15)) {
        vec.z = -225.0f;
    }
    if (is_in_vehicle) {
        if (vec.z > -225.0f) {
            vec.z += 3.0f; // provide higher height for vehicles
        }
        const uintptr_t pos_address_car = FindPointer(WorldPtrBaseAddr, OffsetCarPos);
        const uintptr_t nav_address_car = FindPointer(WorldPtrBaseAddr, OffsetCarNav);
        WriteMemory<v3>(nav_address_car, vec);
        WriteMemory<v3>(pos_address_car, vec);
    } else {
        if (vec.z > -225.0f) {
            vec.z += 0.6f;
        }
        const uintptr_t pos_address = FindPointer(WorldPtrBaseAddr, OffsetPlayerPos);
        const uintptr_t nav_address = FindPointer(WorldPtrBaseAddr, OffsetPlayerNav);
        WriteMemory<v3>(nav_address, vec);
        WriteMemory<v3>(pos_address, vec);
    }
}

int main() {
    SetConsoleTitle(L"Nyaa");

    LPCSTR SignatureWorldPTR = "\x92\x74\xDF\xDA\xDA\xDA\xDA\x9F\xDA\xDA\xDA\xDA\x92\x51\x92\xD2\x92\x5F\x13\xAE\xDD";
    LPCSTR MaskWorldPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\x87\x87\x87\x87\x87";

    LPCSTR SignatureBlipPTR = "\x96\x72\xDF\xDA\xDA\xDA\xDA\xD5\x6D\x1B";
    LPCSTR MaskBlipPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\x87\x87";

    LPCSTR SignaturePlayerPTR = "\x92\x74\xD7\xDA\xDA\xDA\xDA\x32\xDA\xDA\xDA\xDA\x92\x51\x12\x32\xDA\xDA\xDA\xDA\x92\x51\x15";
    LPCSTR MaskPlayerPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\xC0\xC0\xC0\xC0\x87\x87\x87";

    LPCSTR SignatureGlobalPTR = "\x96\x72\xDF\xDA\xDA\xDA\xDA\x97\x51\xD2\x97\x5F\x13\xAE\xCB";
    LPCSTR MaskGlobalPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\x87\x87\x87\x87";

    if (GetProcess(L"GTA5.exe")) {
        Module mod = GetModule(L"GTA5.exe");

        DWORD64 TempWorldPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureWorldPTR, MaskWorldPTR);
        WorldPTR = TempWorldPTR + ReadMemory<DWORD>(TempWorldPTR + 3) + 7;

        DWORD64 TempBlipPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureBlipPTR, MaskBlipPTR);
        BlipPTR = TempBlipPTR + ReadMemory<DWORD>(TempBlipPTR + 3) + 7;

        DWORD64 TempPlayerPTR = FindSignature(mod.dwBase, mod.dwSize, SignaturePlayerPTR, MaskPlayerPTR);
        PlayerPTR = TempPlayerPTR + ReadMemory<DWORD>(TempPlayerPTR + 3) + 7;

        DWORD64 TempGlobalPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureGlobalPTR, MaskGlobalPTR);
        GlobalPTR = TempGlobalPTR + ReadMemory<DWORD>(TempGlobalPTR + 3) + 7;
    }

    cout << "" << endl;
    cout << "Some functions may not work for the steam version." << endl;
    cout << "" << endl;
    std::cout << "press F2 to enable No AFK!" << std::endl;
    std::cout << "press F3 to remove your wanted level!" << std::endl;
    std::cout << "press F4 to enable god mode!" << std::endl;
    std::cout << "press F5 to enable no ragdoll!" << std::endl;
    std::cout << "press F6 to teleport on the waypoint!" << std::endl;
    std::cout << "press F7 to teleport on the objective!" << std::endl;
    std::cout << "press F8 to teleport on the cargo!" << std::endl;
    std::cout << "press F9 to teleport on the terrorbyte!" << std::endl;
    std::cout << "" << std::endl;

    WorldPtrBaseAddr = static_cast<uintptr_t>(WorldPTR);
    BlipPtrBaseAddr = static_cast<uintptr_t>(BlipPTR);
    AfkPtrBaseAddr = static_cast<uintptr_t>(GlobalPTR + 0x8);

    /*========================================================================================*/
    uintptr_t AFKAddress = FindPointer(AfkPtrBaseAddr, OffsetsAFK);
    uintptr_t WantedAddress = FindPointer(WorldPtrBaseAddr, OffsetsWantedLevel);
    uintptr_t GodmodeAddress = FindPointer(WorldPtrBaseAddr, OffsetsGodMode);
    uintptr_t GodmodeAddresstwo = FindPointer(WorldPtrBaseAddr, OffsetsGodModetwo);
    uintptr_t RagdollAddress = FindPointer(WorldPtrBaseAddr, OffsetsRagdoll);
    uintptr_t CarGodModeAddress = FindPointer(WorldPtrBaseAddr, OffsetsCarGodMode);

    cout << "Ragdoll Address = 0x" << hex << RagdollAddress << endl;
    cout << "Godmode Address = 0x" << hex << GodmodeAddress << endl;
    cout << "Wanted Address = 0x" << hex << WantedAddress << endl;
    cout << "AFK Address = 0x" << hex << AFKAddress << endl;

    std::cout << "" << std::endl;
    /*========================================================================================*/
    bool bNoAFK = false;
    bool bNoWanted = false;
    bool bRagdoll = false;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (TRUE) {
        Sleep(1);
        if (GetAsyncKeyState(VK_F2) & 1) {
            bNoAFK = !bNoAFK;
            if (bNoAFK) {
                std::cout << "Anti AFK: on" << std::endl;
                WriteMemory(AFKAddress, 1234567890);
            } else {
                std::cout << "Anti AFK: off" << std::endl;
                WriteMemory(AFKAddress, 10);
            }
        }
        if (GetAsyncKeyState(VK_F3) & 1) {
            bNoWanted = !bNoWanted;
            if (bNoWanted) {
                std::cout << "Removed wanted level" << std::endl;
                WriteMemory(WantedAddress, 0);
                bNoWanted = !bNoWanted;
            }
        }
        if (GetAsyncKeyState(VK_F4) & 1) {
            std::cout << "Godmode: on" << std::endl;
            WriteMemory(GodmodeAddresstwo, true);
            WriteMemory(GodmodeAddress, 16385.0f);
            WriteMemory(CarGodModeAddress, true);
        }
        if (GetAsyncKeyState(VK_F5) & 1) {
            bRagdoll = !bRagdoll;
            if (bRagdoll) {
                std::cout << "No Ragdoll: on" << std::endl;
                WriteMemory(RagdollAddress, true);
            } else {
                std::cout << "No Ragdoll: off" << std::endl;
                WriteMemory(RagdollAddress, false);
            }
        }
        if (GetAsyncKeyState(VK_F6) & 1) {
            auto vec = v3(0, 0, -225.0f);
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                auto color = ReadMemory<DWORD>(n + 0x48);
                if (n > 0 && 8 == type && 84 == color) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &vec, 8, nullptr);
                    teleport(vec);
                    cout << "Teleported to waypoint" << endl;
                    break;
                }
            }
        }
        if (GetAsyncKeyState(VK_F7) & 1) {
            auto vec = v3(0, 0, -225);
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                auto color = ReadMemory<DWORD>(n + 0x48);
                if (n > 0 && type == 1 && (color == 5 || color == 60 || color == 66)) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &vec, sizeof vec, nullptr);
                    if (vec.x != 0 && vec.y != 0) {
                        teleport(vec);
                        cout << "Teleported to objective" << endl;
                        break;
                    }
                    std::cout << "Found Blip: " << std::dec << type << " color " << color << " at x:" << vec.x << " y:"
                              << vec.y
                              << std::endl;
                }
            }
        }
        if (GetAsyncKeyState(VK_F8) & 1) {
            auto vec = v3(0, 0, -225);
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                if (n > 0 && type == 478) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &vec, sizeof vec, nullptr);
                    teleport(vec);
                    cout << "Teleported to cargo" << endl;
                    break;
                }
            }
        }
        if (GetAsyncKeyState(VK_F9) & 1) {
            auto vec = v3(0, 0, -225);
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                if (n > 0 && type == 632) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &vec, sizeof vec, nullptr);
                    teleport(vec);
                    cout << "Teleported to terrorbyte" << endl;
                    break;
                }
            }
        }
        if (GetAsyncKeyState(VK_F12) & 1) {
            float X, Y;
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                auto color = ReadMemory<DWORD>(n + 0x48);
                if (n > 0) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &X, sizeof X, nullptr);
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x14), &Y, sizeof Y, nullptr);

                    std::cout << "Found Blip: " << std::dec << type << " color " << color << " at x:" << X << " y:" << Y
                              << std::endl;
                }
            }
        }
    }
#pragma clang diagnostic pop
}
