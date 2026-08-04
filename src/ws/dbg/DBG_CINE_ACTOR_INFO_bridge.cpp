/* Flattened boundary definitions for the two DBG_CINE_ACTOR_INFO methods DBG_CINE_INFO.h exposes to
 * the HCEX bridge: the (path,startTime) constructor and DumpMatrix. Both open the actor's capture
 * file through fioFILE_DISK and stream doubles into it. Bodies reproduce the decompiled ws-engine
 * methods (addresses noted per function). */

#include "../../headers/hcex/DBG_CINE_INFO.h" /* flat decls + DBG_CINE_ACTOR_INFO / dsTSTRING shims */
#include "../../headers/ws/fio/fioFILE_DISK.h"
#include "../../headers/ws/m3d/m3dMATR.h"

/* DBG_CINE_ACTOR_INFO::DBG_CINE_ACTOR_INFO(const dsTSTRING<char>&, float) @ 0x823B4B00 — share the
 * caller's path buffer, mark the record un-refreshed, then create/truncate the capture file and
 * write the cinematic start time (as a double) as its first record. */
void DBG_CINE_ACTOR_INFO_ctor(DBG_CINE_ACTOR_INFO *self, const dsTSTRING<char> *path, float startTime)
{
    self->filePath.pBuffer = 0;
    self->filePath.pBuffer = path->pBuffer; /* share the caller's buffer (decompiler cast dropped) */
    ++path->pBuffer->refCount;
    self->wasUpdatedCurFrame = 0;

    fioFILE_DISK file;
    if (file.InitFile(path->pBuffer->str, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_W}, -1))
    {
        double startTimeRec = startTime; /* widened to 8 bytes, matching the raw decompile's write */
        file.WriteData(&startTimeRec, 8, 8);
        file.Term();
    }
    /* fioFILE_DISK destructor runs on scope exit (matches the trailing ~fioFILE_DISK). */
}

/* DBG_CINE_ACTOR_INFO::DumpMatrix(const m3dMATR&) @ 0x823B3938 — append this frame's world matrix to
 * the actor's capture file: open for append, then write all 16 matrix elements, each widened to a
 * double. */
void DBG_CINE_ACTOR_INFO_DumpMatrix(DBG_CINE_ACTOR_INFO *self, const void *matrL2W)
{
    const m3dMATR *matr = (const m3dMATR *)matrL2W;

    fioFILE_DISK file;
    if (file.InitFile(self->filePath.pBuffer->str, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_A}, -1))
    {
        for (int i = 0; i < 16; ++i)
        {
            double element = matr->elements[i]; // m3dMATR storage union is anonymous
            file.WriteData(&element, 8, 8);
        }
        file.Term();
    }
}
