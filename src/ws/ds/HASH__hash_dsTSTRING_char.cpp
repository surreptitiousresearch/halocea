// FUNCTION_INDEX entry: HASH__hash_dsTSTRING_char @0x82531EB8 (??$hash@V?$dsTSTRING@D@@@HASH@ds@@SAHABV?$dsTSTRING@D@@II@Z)
#include "MAP.h" // declares ds::HASH (hash<K> template) alongside ds::CMP/FindPrime/BIG_PRIME
#include "dsTSTRING.h"
#include "dsTSTRING_BUF_HEADER.h"

/* CAVEAT: the decompile ("output may be wrong" not flagged, but heavily corrupted by 64-bit
 * OVERLAPPED register packing on this 32-bit-int-returning function) computes each rolling-hash
 * step as a 64-bit `__int64`/`unsigned __int64` whose HIGH dword is, in every case, just the
 * address of the on-stack accumulator (`&back_chain`) or an unrelated loop counter reused via
 * register-pressure aliasing (e.g. `HIDWORD(v4) = j` before `v4` is read as the divisor two lines
 * later) -- classic PowerPC 64-bit-GPR-pair decompiler corruption, not real 64-bit arithmetic on
 * a platform with no native 64-bit int hash. The LOW dwords are internally consistent and are
 * reproduced faithfully below:
 *   - loop body: acc ^= (chunk * a + b) << rotate;  rotate = (rotate + 1) & 7;
 *   - finalization: `1431655777` (0x5555_5561) appears as the low dword of a __PAIR64__ whose
 *     high dword is the unrelated loop-exit counter `j`; the surrounding division/multiply-back
 *     idiom (`x - k*(x/k)`) is arithmetically just `x % k` for unsigned x, k -- reproduced as a
 *     plain modulo against that same constant. */
namespace ds {

// Signature matches the canonical ds::HASH declaration in MAP.h: `template<class K> static
// unsigned int hash(const K &key, unsigned int seedA, unsigned int seedB);`.
template<>
unsigned int HASH::hash<dsTSTRING<char> >(const dsTSTRING<char> &key, unsigned int a, unsigned int b)
{
    dsTSTRING_BUF_HEADER<char> *buffer = key.pBuffer;
    int strLen = buffer->strLen;
    const char *str = buffer->str;

    unsigned int accumulator = 0;
    int rotate = 0;

    int dwordCount = strLen >> 2;
    for (int i = 0; i < dwordCount; ++i) {
        unsigned int chunk = *reinterpret_cast<const unsigned int *>(&str[4 * i]);
        accumulator ^= (chunk * a + b) << rotate;
        rotate = (rotate + 1) & 7;
    }

    for (int byteIndex = 4 * dwordCount; byteIndex < strLen; ++byteIndex) {
        unsigned int byteValue = static_cast<unsigned char>(str[byteIndex]);
        accumulator ^= (byteValue * a + b) << rotate;
        rotate = (rotate + 1) & 7;
    }

    const unsigned int finalizationConstant = 1431655777u; // 0x55555561 — see CAVEAT above
    return accumulator % finalizationConstant;
}

} // namespace ds
