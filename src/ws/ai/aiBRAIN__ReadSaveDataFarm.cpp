#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"      // st.pointee->die
#include "../../headers/ws/ent/entENTITY.h"    // base entENTITY::ReadSaveDataFarm
#include "../../headers/ws/fio/fioFILE.h"       // fioFILE::ReadNextChunk
#include "../../headers/ws/fio/fioCHUNK.h"
#include <string.h>

// aiBRAIN::ReadSaveDataFarm(fioFILE*) @ 0x832ADF98
// ?ReadSaveDataFarm@aiBRAIN@@UAAXPAVfioFILE@@@Z
//
// Restore the base entity farm data, then consume the "died on farm" marker chunk(s) (id 0x122 =
// 290): each one sets the brain's die "restored-dead" bit (0x100000). Stops at the first non-marker
// chunk or end-of-stream.
void aiBRAIN::ReadSaveDataFarm(fioFILE *stream)
{
    entENTITY::ReadSaveDataFarm(stream);

    fioCHUNK chunk;
    memset(&chunk.offsetEnd, 0xFF, 12);
    for (chunk.id = 0; stream->ReadNextChunk(&chunk); st.pointee->die.val |= 0x100000u)
    {
        if (chunk.id != 290)
            break;
    }
}
