#include "../../headers/ws/ent/entDESC.h"

// entDESC::GetNameTplDefault @ 0x8269F9F0
// Pointer to the character data of the default name-template string. `str` is the flexible
// char[1] array tailing dsTSTRING_BUF_HEADER, so this decays to the address right after the
// header (no extra indirection) — confirmed against disasm (`lwz pBuffer; addi r3, pBuffer, 0xC`).
char *entDESC::GetNameTplDefault() const
{
    return nameTplDefault.pBuffer->str;
}
