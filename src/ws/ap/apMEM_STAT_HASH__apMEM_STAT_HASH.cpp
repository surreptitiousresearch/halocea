#include "apMEM_STAT_HASH.h"
#include "apMEM_BLOCK_TYPE.h"

// apMEM_STAT_HASH::apMEM_STAT_HASH @ 0x82C4C810
//
// CAVEAT: the raw decompile walks the flat 128*1024-entry array with a `this - 2`-shifted char
// pointer and strided DWORD/WORD stores (a compiler-optimized fill loop over the 16-byte
// apMEM_ALLOC_INFO stride, not a real "this - 2" pointer). It then repeats a second, differently
// shaped nested 128 x 1024 loop that re-zeroes only `memBlock` and zeroes `free_block[row]`.
// Reconstructed as one straightforward field-by-field slot init (memBlock/memSize/fileName/
// fileLine cleared, type = AP_MEM_BT_NA) plus the free_block reset -- same observable result,
// without reproducing the raw pointer-arithmetic trick.
apMEM_STAT_HASH::apMEM_STAT_HASH()
{
    for (int row = 0; row < 128; ++row)
    {
        for (int slot = 0; slot < 1024; ++slot)
        {
            apMEM_ALLOC_INFO &info = this->allocInfo[row][slot];
            info.memBlock = nullptr;
            info.memSize = 0;
            info.fileName = nullptr;
            info.fileLine = 0;
            info.type = (char)AP_MEM_BT_NA;
        }
        this->free_block[row] = 0;
    }
}
