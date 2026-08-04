#include "../../headers/ws/gs/gsSTRINGS.h"

// 0x826B83A8 — id -> mapIdxToString[id] (an offset into storageStrings) -> wide string pointer
// at that offset.
const wchar_t *gsSTRINGS::GetStringById(int id)
{
    int *storageStringsOffset = &mapIdxToString[id];
    return &storageStrings[*storageStringsOffset];
}
