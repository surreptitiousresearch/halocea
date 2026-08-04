#pragma once

/* D3D9 adapter identifier (external library boundary type). DB-verified
 * (types_members _D3DADAPTER_IDENTIFIER9, 1104 bytes, align(8)). */
#include "_LARGE_INTEGER.h"
#include "guid.h"

typedef struct _D3DADAPTER_IDENTIFIER9
{
    char Driver[512];             /* 0x000 */
    char Description[512];        /* 0x200 */
    char DeviceName[32];          /* 0x400 */
    _LARGE_INTEGER DriverVersion; /* 0x420 */
    unsigned int VendorId;        /* 0x428 */
    unsigned int DeviceId;        /* 0x42C */
    unsigned int SubSysId;        /* 0x430 */
    unsigned int Revision;        /* 0x434 */
    _GUID DeviceIdentifier;       /* 0x438 */
    unsigned int WHQLLevel;       /* 0x448 */
    unsigned char _pad44C[4];     /* 0x44C — align(8) tail */
} _D3DADAPTER_IDENTIFIER9; /* 1104 bytes */
