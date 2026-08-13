#include "headers/ws/ap/apMEM_DUMPER_BASE.h"
#include "headers/ws/ap/apDEFRAG_MNG.h"
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

// Global no-memory-left report dumper. DB type is ?_apNoMemDumper@@3VapMEM_DUMPER@@A
// (a derived apMEM_DUMPER); only the apMEM_DUMPER_BASE slice is touched here, matching the
// decompiler's use of &_apNoMemDumper as an apMEM_DUMPER_BASE*.
extern struct apMEM_DUMPER_BASE _apNoMemDumper;

// ?apDefragMng@@3PAVapDEFRAG_MNG@@A -- the (optional) video-memory defragmenting allocator.
extern struct apDEFRAG_MNG *apDefragMng;

// Set by dlDumpNoMemLeft before it forwards here; suppresses the vid-mem dump on the dl path.
extern int noMemLeftInDl;

// 0x826C0900 -- ?apDumpNoMemLeft@@YAXXZ
// Emit the out-of-memory diagnostics: force the no-mem dumper to echo to the debug console and
// write "no_mem_left.txt" (all report categories, mask 0xF); then, unless we were reached from
// the dl allocator path, dump the defrag allocator's video-memory stats to "vid_mem_dump.txt".
void apDumpNoMemLeft(void)
{
    _apNoMemDumper.dumpToDbgOut = 1;
    _apNoMemDumper.Dump("no_mem_left.txt", (1u << AP_MEM_BT_STATIC) | (1u << AP_MEM_BT_ALLOCATOR) | (1u << AP_MEM_BT_SYSTEM) | (1u << AP_MEM_BT_RSX));

    if (!noMemLeftInDl && apDefragMng)
        apDefragMng->DumpStat("vid_mem_dump.txt");
}
