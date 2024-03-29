#ifndef TESTV_OFFSETS_H
#define TESTV_OFFSETS_H

#include <Windows.h>
#include <vector>

using std::vector;

vector<unsigned int> OffsetsAFK = {0x2C0};
// player -> player info -> wanted level
vector<unsigned int> OffsetsWantedLevel = {0x8, 0x10C8, 0x888};
vector<unsigned int> OffsetsGodMode = {0x8, 0x189};
vector<unsigned int> OffsetsHealth = {0x8, 0x280};
vector<unsigned int> OffsetsRagdoll = {0x8, 0x10B8};
vector<unsigned int> OffsetPlayerNav = {0x8, 0x30, 0x50};
vector<unsigned int> OffsetPlayerPos = {0x8, 0x90};
vector<unsigned int> OffsetIsInVehicle = {0x8, 0x1477};
// player -> vehicle -> pos -> pos
vector<unsigned int> OffsetCarPos = {0x8, 0xD30, 0x30, 0x50};
// player -> vehicle -> pos -> nav
vector<unsigned int> OffsetCarNav = {0x8, 0xD30, 0x30, 0x90};
vector<unsigned int> OffsetsCarGodMode = {0x8, 0xD30, 0x189};

#endif //TESTV_OFFSETS_H
