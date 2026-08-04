#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsTSTRING.h"

struct gsDOM_SPAWN; // spawn domain (entENTITY::pDomSpawn pointee)  boundary

// idasql: byte_8200155A — shared fallback name literal (the "?" / empty-name string).
extern char byte_8200155A[];

// aiBRAIN::implGetNameSpawnedFrom @ 0x83264DC0
// ?implGetNameSpawnedFrom@aiBRAIN@@UAA?AV?$dsTSTRING@D@@XZ (virtual, returns dsTSTRING by value)
// If this brain was created by a spawn domain (entENTITY::pDomSpawn @0x154), ask that domain for
// its name via its virtual slot +48 (0x836..); otherwise return the shared fallback name.
dsTSTRING<char> aiBRAIN::implGetNameSpawnedFrom()
{
    dsTSTRING<char> result;

    gsDOM_SPAWN *dom = this->pDomSpawn; // entENTITY base member @0x154
    if (dom)
    {
        // Virtual dispatch on the domain: vtable slot at byte +48 returns its name by value.
        typedef void (*GetDomNameFn)(dsTSTRING<char> *ret, gsDOM_SPAWN *self);
        GetDomNameFn fn = *(GetDomNameFn *)(*(char **)dom + 48);
        fn(&result, dom);
    }
    else
    {
        result.pBuffer = nullptr;
        result.UnsafeInit(byte_8200155A, -1, 0);
    }

    return result;
}
