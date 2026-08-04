#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"      // st.pointee->die
#include "../../headers/ws/ent/entENTITY.h"    // base entENTITY::WriteSaveDataFarm
#include "../../headers/ws/fio/fioFILE.h"       // fioFILE::WriteChunk / WriteLastChunk / descrList
#include "../../headers/ws/fio/fioCHUNK.h"
#include "../../headers/ws/fio/fioCHUNK_DESCR.h"
#include <string.h>

// aiBRAIN::WriteSaveDataFarm(fioFILE*) @ 0x832ADED0
// ?WriteSaveDataFarm@aiBRAIN@@UAAXPAVfioFILE@@@Z
//
// Persist the base entity farm data, then — if this brain is dead and not flagged BLEED (die bit
// 0x2) — write a marker chunk (id 0x122) recording that the unit died. Back-patches the chunk
// descriptor's end offset (unless the stream is read-only, state bit 0x10) and closes the chunk.
void aiBRAIN::WriteSaveDataFarm(fioFILE *stream)
{
    entENTITY::WriteSaveDataFarm(stream);

    if (IsDead() && ((st.pointee->die.val >> 1) & 1) == 0)
    {
        fioCHUNK chunk;
        memset(&chunk.offsetEnd, 0xFF, 12); // offsetEnd / offsetStart / descrID = -1
        chunk.id = 0;
        stream->WriteChunk(0x122, &chunk);

        if (((stream->state.val >> 4) & 1) == 0)
            stream->descrList[chunk.descrID].offsetEnd = stream->offset;

        chunk.offsetStart = -1;
        chunk.descrID = -1;
        stream->WriteLastChunk();
    }
}
