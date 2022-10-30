#ifndef TESTV_OFFSETS_H
#define TESTV_OFFSETS_H

#include <Windows.h>
#include <vector>

using std::vector;

vector<unsigned int> OffsetsAFK = {0x2C0};
// CPed -> CPlayerInfo -> wanted level
vector<unsigned int> OffsetsWantedLevel = {0x8, 0x10A8, 0x888};
vector<unsigned int> OffsetsGodMode = {0x8, 0x189};
vector<unsigned int> OffsetsHealth = {0x8, 0x280};
vector<unsigned int> OffsetsRagdoll = {0x8, 0x1098};
// CPed -> CNavigation -> pos
vector<unsigned int> OffsetPlayerNav = {0x8, 0x30, 0x50};
// CPed -> vec3
vector<unsigned int> OffsetPlayerPos = {0x8, 0x90};
vector<unsigned int> OffsetIsInVehicle = {0x8, 0x14BB};
// CPed -> CVehicle -> CNavigation -> pos
vector<unsigned int> OffsetCarPos = {0x8, 0xD10, 0x30, 0x50};
// CPed -> CVehicle -> CNavigation -> vec3
vector<unsigned int> OffsetCarNav = {0x8, 0xD10, 0x30, 0x90};
vector<unsigned int> OffsetsCarGodMode = {0x8, 0xD10, 0x189};

#endif //TESTV_OFFSETS_H
