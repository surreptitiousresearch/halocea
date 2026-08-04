#pragma once

/* Havok's byte-sized boolean wrapper (returned by value/pointer from many
   query helpers so the ABI stays uniform across platforms). */
typedef struct hkBool
{
    char m_bool;
} hkBool;
