#include "vector.h"
#include "offsets.h"
#include "memory.h"
#include <iostream>
#include <string>

using namespace std;
using std::cout;
using std::endl;
using std::string;

uintptr_t GlobalPtrBaseAddr;
uintptr_t WorldPtrBaseAddr;
uintptr_t PlayerPtrBaseAddr;
uintptr_t BlipPtrBaseAddr;
uintptr_t AfkPtrBaseAddr;

uintptr_t AFKAddress;
uintptr_t WantedAddress;
uintptr_t GodmodeAddress;
uintptr_t GodmodeAddressTwo;
uintptr_t RagdollAddress;

void findBaseAddr() {
    Module mod = TargetModule;
    LPCSTR SignatureWorldPTR = "\x92\x74\xDF\xDA\xDA\xDA\xDA\x9F\xDA\xDA\xDA\xDA\x92\x51\x92\xD2\x92\x5F\x13\xAE\xDD";
    LPCSTR MaskWorldPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\x87\x87\x87\x87\x87";

    LPCSTR SignatureBlipPTR = "\x96\x72\xDF\xDA\xDA\xDA\xDA\xD5\x6D\x1B";
    LPCSTR MaskBlipPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\x87\x87";

    LPCSTR SignaturePlayerPTR = "\x92\x74\xD7\xDA\xDA\xDA\xDA\x32\xDA\xDA\xDA\xDA\x92\x51\x12\x32\xDA\xDA\xDA\xDA\x92\x51\x15";
    LPCSTR MaskPlayerPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\xC0\xC0\xC0\xC0\x87\x87\x87";

    LPCSTR SignatureGlobalPTR = "\x96\x72\xDF\xDA\xDA\xDA\xDA\x97\x51\xD2\x97\x5F\x13\xAE\xCB";
    LPCSTR MaskGlobalPTR = "\x87\xA2\x87\xC0\xC0\xC0\xC0\x87\x87\x87\x87\x87\x87\x87\x87";

    DWORD64 TempWorldPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureWorldPTR, MaskWorldPTR);
    WorldPtrBaseAddr = TempWorldPTR + ReadMemory<DWORD>(TempWorldPTR + 3) + 7;

    DWORD64 TempBlipPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureBlipPTR, MaskBlipPTR);
    BlipPtrBaseAddr = TempBlipPTR + ReadMemory<DWORD>(TempBlipPTR + 3) + 7;

    DWORD64 TempPlayerPTR = FindSignature(mod.dwBase, mod.dwSize, SignaturePlayerPTR, MaskPlayerPTR);
    PlayerPtrBaseAddr = TempPlayerPTR + ReadMemory<DWORD>(TempPlayerPTR + 3) + 7;

    DWORD64 TempGlobalPTR = FindSignature(mod.dwBase, mod.dwSize, SignatureGlobalPTR, MaskGlobalPTR);
    GlobalPtrBaseAddr = TempGlobalPTR + ReadMemory<DWORD>(TempGlobalPTR + 3) + 7;

    AfkPtrBaseAddr = GlobalPtrBaseAddr + 0x8;

    /*========================================================================================*/
    AFKAddress = FindPointer(AfkPtrBaseAddr, OffsetsAFK);
    WantedAddress = FindPointer(WorldPtrBaseAddr, OffsetsWantedLevel);
    GodmodeAddress = FindPointer(WorldPtrBaseAddr, OffsetsGodMode);
    GodmodeAddressTwo = FindPointer(WorldPtrBaseAddr, OffsetsGodModeTwo);
    RagdollAddress = FindPointer(WorldPtrBaseAddr, OffsetsRagdoll);
}

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

    if (!GetProcess(L"GTA5.exe")) {
        return -10086;
    }

    GetModule(L"GTA5.exe");
    findBaseAddr();

    cout << "" << endl;
    cout << "Some functions may not work for the steam version." << endl;
    cout << "" << endl;
    std::cout << "press F2 to enable No AFK!" << std::endl;
    std::cout << "press F3 to remove your wanted level!" << std::endl;
    std::cout << "press F4 to enable god mode!" << std::endl;
    std::cout << "press F5 to enable no ragdoll!" << std::endl;
    std::cout << "press F7 to teleport!" << std::endl;
    std::cout << "" << std::endl;

    cout << "Ragdoll Address = 0x" << hex << RagdollAddress << endl;
    cout << "Godmode Address = 0x" << hex << GodmodeAddress << endl;
    cout << "Wanted Address = 0x" << hex << WantedAddress << endl;
    cout << "AFK Address = 0x" << hex << AFKAddress << endl;

    std::cout << "" << std::endl;
    /*========================================================================================*/
    bool bNoAFK = false;
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
            std::cout << "Removed wanted level" << std::endl;
            WriteMemory(WantedAddress, 0);
        }
        if (GetAsyncKeyState(VK_F4) & 1) {
            std::cout << "Godmode: on" << std::endl;
            WriteMemory(GodmodeAddressTwo, static_cast<BYTE>(true));
            WriteMemory(GodmodeAddress, 16385.0f);
            const uintptr_t is_in_vehicle_address = FindPointer(WorldPtrBaseAddr, OffsetIsInVehicle);
            const BOOL is_in_vehicle = ReadMemory<BYTE>(is_in_vehicle_address) == 0;
            if (is_in_vehicle) {
                const uintptr_t car_god_mode_address = FindPointer(WorldPtrBaseAddr, OffsetsCarGodMode);
                WriteMemory(car_god_mode_address, static_cast<BYTE>(true));
            }
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
        if (GetAsyncKeyState(VK_F7) & 1) {
            DWORD self_color = 0;
            auto objective_found = false;
            auto cargo_found = false;
            auto terrorbyte_found = false;
            auto vec = v3(0, 0, -225.0f);
            for (int i = 0; i < 2000; i++) {
                auto blip = ReadMemory<uintptr_t>(BlipPtrBaseAddr + i * 8);
                if (blip > 0) {
                    auto type = ReadMemory<DWORD>(blip + 0x40);
                    auto color = ReadMemory<DWORD>(blip + 0x48);
#ifdef DEBUG
                    std::cout << "Found Blip: " << std::dec << type << " color " << color << " at x:" << vec.x << " y:"
                              << vec.y
                              << std::endl;
#endif
                    if (type == 6) {
                        auto color_g = (color >> 16) & 0xff;
                        auto color_b = (color >> 8) & 0xff;
                        if (color_g != color_b) {
                            self_color = color;
                            cout << "self color:" << hex << color << endl;
                        }
                    }
                    if (8 == type && 84 == color) {
                        vec = ReadMemory<v3>(blip + 0x10);
                        vec.z = -225.0f;
                        teleport(vec);
                        cout << "Teleported to waypoint" << endl;
                        goto exit_teleport;
                    }
                    if (type == 1 && (color == 5 || color == 60 || color == 66)) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            objective_found = true;
                        }
                    }
                    if (!objective_found && (type == 478 || type == 556)) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            cargo_found = true;
                        }
                    }
                    if (!objective_found && !cargo_found && type == 632 && color == self_color) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            terrorbyte_found = true;
                        }
                    }
                }
            }
            if (objective_found) {
                teleport(vec);
                cout << "Teleported to objective" << endl;
                continue;
            }
            if (cargo_found) {
                teleport(vec);
                cout << "Teleported to cargo" << endl;
                continue;
            }
            if (terrorbyte_found) {
                teleport(vec);
                cout << "Teleported to terrorbyte" << endl;
                continue;
            }
            exit_teleport:;
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

                    std::cout << "Found Blip: " << std::dec << type << " color " << hex << color << " at x:" << dec << X
                              << " y:" << Y
                              << std::endl;
                }
            }
        }
    }
#pragma clang diagnostic pop
}
