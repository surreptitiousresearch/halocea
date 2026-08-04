#include "mtlMTL_MNG.h"

// zlib CRC32 — boundary, matches the corpus convention for leaf CRT/zlib primitives.
extern "C" unsigned int crc32(unsigned int crc, const unsigned char *buf, unsigned int len); // boundary
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...); // boundary

// mtlMTL_MNG::_MtlSuffixFromAffixString(const dsTSTRING<char> &) const — 0x82A86A50
// Hash affixStr down to an 8-hex-digit CRC32 suffix. An empty input adopts the shared
// empty-string singleton instead of hashing.
// ABI: r3 = sret result buffer, r4 = this (loaded, verified unused by disasm), r5 = affixStr.
dsTSTRING<char> mtlMTL_MNG::_MtlSuffixFromAffixString(const dsTSTRING<char> &affixStr) const
{
    dsTSTRING<char> result;

    unsigned int affixLen = affixStr.pBuffer->strLen;
    if (affixLen)
    {
        const char *affixText = affixStr.pBuffer->str;
        unsigned int crc = crc32(0, nullptr, 0);
        crc = crc32(crc, (const unsigned char *)affixText, affixLen);
        dsSPrintf(&result, "%08X", crc);
        return result;
    }

    // Empty input: adopt the shared empty-string singleton (dsTSTRING<char>::UnsafeInitEmpty
    // inlined — lazily allocates the singleton on first use, then shares/bumps its refcount).
    result.UnsafeInitEmpty();
    return result;
}
