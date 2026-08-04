#pragma once
// ws-engine ld: a serialized Havok MOPP code blob loaded for one collidable object.
// DB-verified layout (types_members ldMOPPCode):
//   objId@0, nFace@4, nVert@8, checksumm@12, header@24, size@44, data@48 — size 52.
#include "../m3d/m3dV.h"
#include "../ds/dsVECTOR.h"

typedef struct ldMOPPCode {
    __int16               objId;    // 0x00 owning object id
    unsigned char         _pad2[2]; // 0x02
    int                   nFace;    // 0x04
    int                   nVert;    // 0x08
    m3dV                  checksumm;// 0x0C geometry checksum vector
    dsVECTOR<float, 8>    header;   // 0x18 MOPP header floats
    unsigned int          size;     // 0x2C code byte size
    unsigned __int8      *data;     // 0x30 MOPP code bytes
} ldMOPPCode;                                  // size 52
