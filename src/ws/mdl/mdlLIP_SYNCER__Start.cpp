// 0x82747018  ?Start@mdlLIP_SYNCER@@QAAXABV?$dsTSTRING@D@@_N@Z
// Begin a lip-sync clip. Reset, transition OFF/EASE_OFF -> EASE_ON, then open the spline file
// "<voPath|exPath>/<name>.spl". Each chunk supplies either a per-phoneme spline (id 0x1E0,
// preceded by a phoneme index 0..4), a 4-byte value (id 0x1E1, ignored here), or the sample rate
// (id 0x1E2). isClosedOnly stays set only if every phoneme spline was the "closed" slot (index 4).
// Finally derive timeEnd from the closed slot's last keypoint arg scaled to seconds.
//
// The decompiler's __SPAIR64__ artifact on the divisor is just sampleRate; verified against disasm.
#include "../../headers/ws/mdl/mdlLIP_SYNCER.h"
#include "../../headers/ws/mdl/mdl_lip_syncer_boundary.h"

void mdlLIP_SYNCER::Start(const dsTSTRING<char> &name, bool isExclamation)
{
    OnReset();

    STATUS st = status;
    if (st == OFF || st == EASE_OFF)
        status = EASE_ON;

    const char *dir = isExclamation ? lipsync::ex_path.pBuffer->str
                                    : lipsync::vo_path.pBuffer->str;

    fioFILE *file = fioCache.OpenFile(dir, name.pBuffer->str, "spl", 9u);
    if (!file) {
        apLogErr("~VOSPLINE~ Can't find lipsync spline for sound %s", name.pBuffer->str);
        return;
    }

    isClosedOnly = true;
    sampleRate = 44100;

    fioCHUNK chunk;
    chunk.offsetEnd = -1;
    chunk.id = 0;
    chunk.offsetStart = -1;
    chunk.descrID = -1;

    if (fioFILE_ReadNextChunk(file, &chunk)) {
        do {
            switch (chunk.id) {
                case 0x1E0: {  // phoneme spline
                    unsigned char phonemeIdx;
                    fioFILE_ReadData(file, &phonemeIdx, 1, 1);
                    if (phonemeIdx >= 5u)
                        return;
                    m3dSPL *old = seqList[phonemeIdx].spline;
                    if (old) {
                        // scalar-deleting dtr: deleteFlag=1 => run ~m3dSPL then free
                        old->__vftable->dtr_m3dSPL(old, 1);
                    }
                    seqList[phonemeIdx].spline = fioFILE_ReadSpline(file);
                    if (phonemeIdx != 4)
                        isClosedOnly = false;
                    break;
                }
                case 0x1E1: {  // unused 4-byte field
                    __int64 scratch;
                    fioFILE_ReadData(file, &scratch, 4, 4);
                    break;
                }
                case 0x1E2:  // sample rate
                    fioFILE_ReadData(file, &sampleRate, 4, 4);
                    break;
                default:
                    return;
            }
        } while (fioFILE_ReadNextChunk(file, &chunk));
    }

    fioCache.CloseFile(file);

    int lastKp = seqList.list[4].spline->nKp - 1;
    int rate = sampleRate;
    timeEnd = (seqList.list[4].spline->GetKpArg(lastKp) * 1024.0f) / (float)rate - 0.050000001f;
}
