#include <string.h>
#include "dsSTRID_POOL.h"
#include "ds_assert_boundary.h"

extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary — dlmalloc allocator

// One string block is 8 KiB; the pool holds up to 256 of them.
enum { STRING_BLOCK_SIZE = 0x2000 };

// dsSTRID_POOL::AllocStr @ 0x825281D8
// Copy `str` into the pool's fixed-size string blocks and return the stable pointer to the copy.
// Fills the current block front-to-back; when the string will not fit, advances to the next block
// (lazily malloc'ing blocks the first time each index is used).
const char *dsSTRID_POOL::AllocStr(const char *str)
{
    int len = (int)strlen(str);

    if (!IGNORE_STRONG_ASSERT && len + 1 >= STRING_BLOCK_SIZE)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "len + 1 < STRING_BLOCK_SIZE",
            "D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp",
            403,
            empty_string);

    if (this->posStorage + len + 1 > STRING_BLOCK_SIZE) {
        // no room left in the current block: move to the next one
        this->posStorage = 0;
        this->idxStorage = this->idxStorage + 1;
        if (!IGNORE_STRONG_ASSERT && this->idxStorage >= 0x100)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "idxStorage < (sizeof(strStorage)/sizeof((strStorage)[0]))",
                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_strid.cpp",
                409,
                empty_string);
    }

    if (!this->strStorage[this->idxStorage]) {
        this->strStorage[this->idxStorage] =
            (char *)dlMalloc(STRING_BLOCK_SIZE, this->_cl.file, this->_cl.line);
        this->posStorage = 0;
    }

    char *dest = &this->strStorage[this->idxStorage][this->posStorage];
    memcpy(dest, str, len + 1);
    this->posStorage += len + 1;
    return dest;
}
