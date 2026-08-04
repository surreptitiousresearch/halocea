#pragma once
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "strmVID_MEM_BLOCK.h"

/* strmVID_MEM_MANAGER — ws-engine strm subsystem: owns the set of strmVID_MEM_BLOCK physical-memory
 * reservations used for streamed video data. DB-verified layout (types_members strmVID_MEM_MANAGER),
 * size 0x4C (76). */

typedef struct strmVID_MEM_MANAGER
{
    dsVECTOR<strmVID_MEM_BLOCK *, 8> blocks;           /* 0x00 */
    osLOCK                            lock;              /* 0x14 */
    int                                blocksCountLimit;  /* 0x48 */

    // 0x8269D5A0 — allocate and Init() blocks one at a time (each a fresh physical-memory
    // reservation) until a block fails to Init, pushing each successful block onto `blocks`;
    // the last (failed) block's dsVECTOR storage is freed and the block itself deleted.
    void ReserveAllMemory();

    // Body lives in the strm subsystem — boundary (called from hcex_change_render_mode). Flush
    // pending block reservations / GPU fences before a render-mode swap.
    void Sync();

    // 0x8269D538 — sum the `size` of every block in `blocks` (allocating a first block via
    // AllocateNewBlock if none exist yet).
    int GetSizeTotal();

/* private: */
    // 0x8269CEA8 — allocate a fresh strmVID_MEM_BLOCK (inlined default construction, matching
    // ReserveAllMemory's pattern) and Init() it with the next `blockId`; on success, push it onto
    // `blocks` and return it. On an Init failure, free the block's dsVECTOR backing storage,
    // delete it, and return nullptr (allocation failure of the block itself is not guarded, same
    // as ReserveAllMemory).
    strmVID_MEM_BLOCK *AllocateNewBlock();
} strmVID_MEM_MANAGER;
