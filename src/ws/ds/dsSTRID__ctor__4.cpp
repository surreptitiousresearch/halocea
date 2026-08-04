#include "dsSTRID.h"
#include "dsSTRID_POOL.h"
#include "dsTSTRING.h"
#include "dsTSTRING_BUF_HEADER.h"

extern dsSTRID_POOL &GetStrIdPool(); // ds global accessor — the process-wide dsStrIdPool singleton

// dsSTRID::dsSTRID(const dsTSTRING<char>&, bool) @ 0x8252B938
// Same as dsSTRID(const char*, bool), but the source characters come from a dsTSTRING<char>:
// measures `s.pBuffer->strLen` instead of scanning for a NUL, then interns `s.pBuffer->str`.
dsSTRID::dsSTRID(const dsTSTRING<char> &s, bool existOnly)
{
    dsSTRID_POOL &pool = GetStrIdPool();
    if (s.pBuffer->strLen) {
        const char *chars = s.pBuffer->str;
        dsSTRID interned = existOnly ? pool.GetIdByStr(chars) : pool.AddStr(chars);
        this->id = interned.id;
    } else {
        // Same reserved-empty-slot read as dsSTRID(const char*, bool); see CAVEAT there.
        this->id = pool.stringTable.Get(0);
    }
}
