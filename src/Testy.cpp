#include "vector.h"
#include "offsets.h"
#include "signatures.h"
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

void findBaseAddr() {
    DWORD64 TempWorldPTR = FindSignature(SignatureWorldPTR, MaskWorldPTR);
    WorldPtrBaseAddr = TempWorldPTR + ReadMemory<DWORD>(TempWorldPTR + 3) + 7;

    DWORD64 TempBlipPTR = FindSignature(SignatureBlipPTR, MaskBlipPTR);
    BlipPtrBaseAddr = TempBlipPTR + ReadMemory<DWORD>(TempBlipPTR + 3) + 7;

    DWORD64 TempPlayerPTR = FindSignature(SignaturePlayerPTR, MaskPlayerPTR);
    PlayerPtrBaseAddr = TempPlayerPTR + ReadMemory<DWORD>(TempPlayerPTR + 3) + 7;

    DWORD64 TempGlobalPTR = FindSignature(SignatureGlobalPTR, MaskGlobalPTR);
    GlobalPtrBaseAddr = TempGlobalPTR + ReadMemory<DWORD>(TempGlobalPTR + 3) + 7;

    AfkPtrBaseAddr = GlobalPtrBaseAddr + 0x8;

    /*========================================================================================*/
    AFKAddress = FindPointer(AfkPtrBaseAddr, OffsetsAFK);
}

BOOL isInVehicle() {
    BOOL isInVehicle = (ReadMemory<BYTE>(FindPointer(WorldPtrBaseAddr, OffsetIsInVehicle)) & 0x10) == 0;
    std::cout << "isInVehicle: " << isInVehicle << std::endl;
    return isInVehicle;
}

void teleport(v3 vec) {
    if (GetAsyncKeyState(VK_SHIFT) & (1 << 15)) {
        vec.z = -225.0f;
    }
    if (isInVehicle()) {
        if (vec.z > -225.0f) {
            vec.z += 3.0f; // provide higher height for vehicles
        }
        const uintptr_t pos_address_car = FindPointer(WorldPtrBaseAddr, OffsetCarPos);
        const uintptr_t nav_address_car = FindPointer(WorldPtrBaseAddr, OffsetCarNav);
        WriteMemory<v3>(nav_address_car, vec);
        WriteMemory<v3>(pos_address_car, vec);
    } else {
        if (vec.z > -225.0f) {
            vec.z += 1.0f;
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

    std::cout << "press F1 to kill GTA!" << std::endl;
    std::cout << "press F2 to enable No AFK!" << std::endl;
    std::cout << "press F3 to remove your wanted level!" << std::endl;
    std::cout << "press F4 to enable god mode!" << std::endl;
    std::cout << "press F5 to enable no ragdoll!" << std::endl;
    std::cout << "press F7 to teleport!" << std::endl;
    std::cout << "" << std::endl;

    cout << "Global Base = 0x" << hex << GlobalPtrBaseAddr << endl;
    cout << "World Base = 0x" << hex << WorldPtrBaseAddr << endl;
    cout << "Blip Base = 0x" << hex << BlipPtrBaseAddr << endl;
    cout << "AFK Address = 0x" << hex << AFKAddress << endl;

    std::cout << "" << std::endl;
    /*========================================================================================*/
    bool bNoAFK = false;
    bool bRagdoll = false;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (TRUE) {
        Sleep(1);
        if (GetAsyncKeyState(VK_F1) & 1) {
            system("taskkill /F /IM GTA5.exe");
            system("taskkill /F /IM Launcher.exe");
            system("taskkill /F /IM RockstarService.exe");
            system("taskkill /F /IM PlayGTAV.exe");
            std::cout << "Killed" << std::endl;
            return 0;
        }
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
            auto wantedLevel = ReadMemory<DWORD>(FindPointer(WorldPtrBaseAddr, OffsetsWantedLevel));
            std::cout << "Wanted Level: " << wantedLevel << std::endl;
            std::cout << "Removed wanted level" << std::endl;
            WriteMemory(FindPointer(WorldPtrBaseAddr, OffsetsWantedLevel), static_cast<DWORD>(0));
        }
        if (GetAsyncKeyState(VK_F4) & 1) {
            std::cout << "Godmode: on" << std::endl;
            WriteMemory(FindPointer(WorldPtrBaseAddr, OffsetsGodMode), static_cast<BYTE>(true));
            WriteMemory(FindPointer(WorldPtrBaseAddr, OffsetsHealth), 16385.0f);
            if (isInVehicle()) {
                WriteMemory(FindPointer(WorldPtrBaseAddr, OffsetsCarGodMode), static_cast<BYTE>(true));
            }
        }
        if (GetAsyncKeyState(VK_F5) & 1) {
            const auto ragdoll_addr = FindPointer(WorldPtrBaseAddr, OffsetsRagdoll);
            bRagdoll = !bRagdoll;
            if (bRagdoll) {
                std::cout << "No Ragdoll: on" << std::endl;
                WriteMemory(ragdoll_addr, true);
            } else {
                std::cout << "No Ragdoll: off" << std::endl;
                WriteMemory(ragdoll_addr, false);
            }
        }
        if (GetAsyncKeyState(VK_F7) & 1) {
            DWORD self_color = 0;
            auto matched_found = false;
            auto objective_found = false;
            auto cargo_found = false;
            auto terrorbyte_found = false;
            auto vec = v3(0, 0, -225.0f);
            for (int i = 0; i < 2000; i++) {
                auto blip = ReadMemory<uintptr_t>(BlipPtrBaseAddr + i * 8);
                if (blip > 0) {
                    auto type = ReadMemory<DWORD>(blip + 0x40);
                    auto color = ReadMemory<DWORD>(blip + 0x48);
                    if (type == 6) {
                        auto color_g = (color >> 16) & 0xff;
                        auto color_b = (color >> 8) & 0xff;
                        if (color_g != color_b) {
                            self_color = color;
                            cout << "self color:" << hex << color << endl;
                        }
                    }
                    // Waypoint
                    if (8 == type && 84 == color) {
                        vec = ReadMemory<v3>(blip + 0x10);
                        vec.z = -225.0f;
                        teleport(vec);
                        cout << "Teleported to waypoint" << endl;
                        goto exit_teleport;
                    }
                    // 760 -> Kosatka
                    if (type == 760 && GetAsyncKeyState('I') & 1) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            matched_found = true;
                        }
                    }
                    // 618 -> Gold, 776 -> Paint, 272 -> Cash, 496 -> Weed, 514 -> Coke
                    if ((type == 618 || type == 776 || type == 272 || type == 496 || type == 514) &&
                        GetAsyncKeyState('G') & 1) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            matched_found = true;
                        }
                    }
                    // 535 -> Letter A => 542 -> Letter H
                    if (535 <= type && type <= 542 && GetAsyncKeyState(type - 535 + 'A') & 1) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            matched_found = true;
                        }
                    }
                    // objective dot
                    if (!matched_found && type == 1 && (color == 5 || color == 60 || color == 66)) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            objective_found = true;
                        }
                    }
                    // objective blips
                    if (!matched_found && (color == 5 || color == 60 || color == 66) &&
                        ((type >= 768 && type <= 776) || type == 679 || type == 743 || type == 760 || type == 619 ||
                         type == 66)) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            objective_found = true;
                        }
                    }
                    // 478 -> Crate, 556 -> Shell, 586 -> Duffle Bag, 615 -> Business Crate
                    if (!matched_found && !objective_found &&
                        (type == 478 || type == 556 || type == 586 || type == 615)) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            cargo_found = true;
                        }
                    }
                    // Terrorbyte
                    if (!matched_found && !objective_found && !cargo_found && type == 632 && color == self_color) {
                        auto temp_vec = ReadMemory<v3>(blip + 0x10);
                        if (temp_vec.x != 0 && temp_vec.y != 0) {
                            vec = temp_vec;
                            terrorbyte_found = true;
                        }
                    }
                }
            }
            if (matched_found) {
                teleport(vec);
                cout << "Teleported to matched objective" << endl;
                continue;
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
#ifdef DEBUG
        if (GetAsyncKeyState(VK_F11) & 1) {
            float X, Y;
            for (int i = 0; i < 2000; i++) {
                auto a = BlipPtrBaseAddr;
                auto n = ReadMemory<uintptr_t>(a + i * 8);
                auto type = ReadMemory<DWORD>(n + 0x40);
                auto color = ReadMemory<DWORD>(n + 0x48);
                if (n > 0) {
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x10), &X, sizeof X, nullptr);
                    ReadProcessMemory(TargetProcess, reinterpret_cast<BYTE *>(n + 0x14), &Y, sizeof Y, nullptr);

                    std::cout << "Found Blip: " << std::dec << type << " color " << std::hex << color << " at x:"
                              << std::dec << X
                              << " y:" << Y
                              << std::endl;
                }
            }
        }
#endif
    }
#pragma clang diagnostic pop
}
