#include "../../headers/ws/ap/apCONFIG.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// boundary — printf-into-dsTSTRING helper (declared in several ws boundary headers).
const dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// 0x823D5E48 — apCONFIG::Retrieve<int,int>. Build the composite key "<section>.<key>", look it up
// via GetValue<int>, and fall back to `defVal` when the key is absent (or when either name is
// null). The scratch dsTSTRING is released by its destructor at scope exit.
// DEVIATION: the decompiler packed section+key into one __int64 and mislabeled the two int refs;
// the disassembly (r4=section, r5=key, r6=&val, r7=&defVal) confirms the shape used here.
template<>
void apCONFIG::Retrieve<int, int>(const char *section, const char *key, int &val, const int &defVal)
{
    if (section && key) {
        dsTSTRING<char> composite;
        dsSPrintf(&composite, "%s.%s", section, key);
        if (!GetValue<int>(composite.pBuffer->str, &val))
            val = defVal;
    } else {
        val = defVal;
    }
}
