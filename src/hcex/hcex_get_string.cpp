/* hcex_get_string @0x823CF0B0 — resolve a localized string by its case-insensitive text id, going
 * through the ws-engine string table (gsSTRINGS). When the HCEX string-debug toggle is on, wraps
 * the resolved text in "'+...+'" markers (into a shared scratch buffer) so debug builds can spot
 * strings routed through this bridge at a glance.
 *
 * DEVIATION: the decompiler emits this with 8 parameters (str_name plus seven uninitialised
 * decompiler locals a2..a8) by mis-attributing GetStringId's `this` (gsStrings, loaded from a
 * global) as extra caller-supplied arguments. The mangled name (`?hcex_get_string@@YAPA_WPBD@Z`)
 * and the disassembly (`GetStringId` call takes only `this` + `idText`) confirm the real signature
 * takes a single argument; corrected here. */

#include <wchar.h>
#include "../headers/ws/gs/gsSTRINGS.h"

extern gsSTRINGS *gsStrings;
extern int        hcex_test_strings;
extern wchar_t     buf[2048];   /* DB-verified size (4096 bytes); shared debug-wrap scratch buffer */

wchar_t *hcex_get_string(const char *str_name)
{
    int id = gsStrings->GetStringId(str_name);
    if (!id)
        return 0;

    if (hcex_test_strings) {
        const wchar_t *resolved = gsStrings->GetStringById(id);
        swprintf(buf, L"'+%s+'", resolved);
        return buf;
    }

    return (wchar_t *)gsStrings->GetStringById(id);
}
