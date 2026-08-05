#pragma once
// Boundary externs for the HALO_SOUND_LIST method cluster (src/hcex/HALO_SOUND_LIST__*.cpp) and
// the HALO_SOUND_PARAMS::CMP comparator. Pulls in the owning type graph (HALO_SOUND_SYSTEM brings
// HALO_SOUND_LIST / HALO_SOUND_TAG / HALO_SOUND_PARAMS + the snd::/FMOD boundary and the
// haloSoundSystem singleton), the shared misc-globals header (dsSPrintf / fnmGetNameExtStr /
// dbg_printBufPlay_10 / os* helpers), the ws-engine ds/fio boundaries the bodies touch, and the
// small set of extra allocator / os-file / sort externs unique to this cluster. Nothing here is
// reversed -- declared only far enough to type call sites; all DB-verified.

#include "HALO_SOUND_SYSTEM.h"
#include "HALO_SOUND_LIST_HALO_CHANNEL_globals.h"  // dsSPrintf, fnmGetNameExtStr, dbg_printBufPlay_10, DBG_VAR<>
// NB: HALO_SOUND_SYSTEM_globals.h is deliberately NOT included -- it re-defines the same DBG_VAR<T>
// template as HALO_SOUND_LIST_HALO_CHANNEL_globals.h (C2953). The two os-layer helpers this cluster
// needs from it are declared narrowly below instead.
#include "../ws/ds/ds_new.h"                         // tracked operator new[](size,file,line)
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsTSTRING.h"
#include "../ws/ds/dsSTRID.h"
#include "../ws/ds/WILDCARD.h"
#include "../ws/ds/MAP.h"
#include "../ws/ds/dsCMP.h"
#include "../ws/ds/dsSWAP.h"
#include "../ws/fio/fioFILE.h"
#include "../ws/fio/fioFILE_DISK.h"
#include "../ws/fio/FIO_OPEN.h"
#include "../ws/os/OS_FILE.h"
#include "../ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../ws/dbg/STRONG_ASSERT2_HELPER.h"

// ---- os-layer helpers (real bodies in src/ws/os/; narrow redeclare, see note above) ----
extern "C" {
    unsigned int osGetCurThreadId();                        // src/ws/os/osGetCurThreadId.cpp
    void         osOutputDebugString(const char *fmt, ...); // boundary
}

// ---- ws-engine dbg boundary globals (shared with the HALO_CHANNEL cluster) ----
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

// ---- ws-engine dlmalloc-family allocator (heap alloc used by the inlined dsTSTRING paths) ----
extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line); // boundary
extern "C" void dlFree(void *ptr);                                              // boundary

// ---- Saber os-layer file I/O wrappers (real bodies in src/ws/os/; boundary here). The mode is a
// dsFLAGS<OS_FILE,int> passed by value; the disasm loads it as a raw int (OS_FILE_READ == 1). ----
struct osFILE_HANDLE_DUMMY;
extern "C" {
    osFILE_HANDLE_DUMMY *osFileOpen(const char *name, dsFLAGS<OS_FILE, int> mode);     // boundary
    int                  osFileGetSize(osFILE_HANDLE_DUMMY *file);                      // boundary
    int                  osFileRead(osFILE_HANDLE_DUMMY *file, void *dst, int size);    // boundary
    void                 osFileClose(osFILE_HANDLE_DUMMY *file);                        // boundary
}

// ---- ds::Sort free function (in-place comparator sort over a [begin,end) iterator range).
// boundary -- reversed elsewhere; only the (name,index) pair instantiation is used here. ----
namespace ds {
template<class Iter, class Cmp>
void Sort(Iter begin, Iter end, const Cmp &cmp); // boundary
}

// The (name,index) instance of the macro-generated ds::NAMED_PAIR field-name family the DB spells
// `ds::NAMED_PAIR<ds::NAMED_PAIR_name<dsTSTRING<char>>, ds::NAMED_PAIR_index<int>>`. The family's
// member names come from its two tag arguments, so each instance is its own concrete struct; this
// is the temporary sub-sound (path-name, fsb-index) record ParseLst sorts and groups into tags.
struct HALO_SOUND_LST_PAIR {
    dsTSTRING<char> name;  // 0x00 lowercased "sound\<name>" relative path
    int             index; // 0x04 FSB sub-sound ordinal (line number)
};

// "sound\" literal ParseLst prefixes every derived tag path with (DB: aSound_1).
extern const char aSound_1[];
