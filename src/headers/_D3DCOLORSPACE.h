#pragma once

/* Xbox 360 D3D color-space enum (external library boundary type). DB-verified
 * (types_enum_values _D3DCOLORSPACE). 4-byte SDK enum. */
typedef enum _D3DCOLORSPACE
{
    D3DCOLORSPACE_RGB         = 0,
    D3DCOLORSPACE_YCbCr601    = 1,
    D3DCOLORSPACE_YCbCr709    = 2,
    D3DCOLORSPACE_FORCE_DWORD = 2147483647,
} _D3DCOLORSPACE; /* 4 bytes */
