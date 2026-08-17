/* ?_UnshareName@objOBJ@@QAAHXZ @0x82A7EBF0 */
#include "objOBJ.h"
// 0x82A7EBF0  ?_UnshareName@objOBJ@@QAAHXZ
// If this object's name is shared (stateShare bit 0x20), replace it with a privately-allocated
// copy of the same string. Returns 1 on success (including the not-shared / no-name cases),
// 0 if the replacement allocation failed.
//
// CAVEAT: the decompiler fabricated trailing phantom register args (a2..a7) — the mangle (XZ)
// confirms none. The original (shared) name buffer is intentionally not freed here.

int objOBJ::_UnshareName()
{
    if ((this->stateShare.state & 0x20) == 0)
        return 1;

    this->stateShare.state &= ~0x20; // clear "name shared" flag

    char *sharedName = this->name;
    if (!this->name)
        return 1;

    char *scan = this->name;
    this->name = nullptr;
    while (*scan++)
        ; // advance to the terminator to measure the length
    int nameLen = (int)(scan - sharedName - 1);

    if (!_AllocName(nameLen))
        return 0;

    // Copy the shared string (including terminator) into the freshly allocated buffer.
    char *src = sharedName - 1;
    char *dst = this->name - 1;
    char ch;
    do {
        ch = *++src;
        *++dst = ch;
    } while (ch);
    return 1;
}
