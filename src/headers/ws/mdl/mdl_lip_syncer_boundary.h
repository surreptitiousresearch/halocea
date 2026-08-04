#pragma once
// Boundary declarations touched by the mdlLIP_SYNCER batch (ctor / Start / ApplyLipSync) that
// live outside the mdl subsystem: the fio cache/file readers, the anim TRS extractor, the m3d
// spline evaluator, the lipsync config globals, and small CRT/log helpers. Only the shapes the
// three functions touch are modeled; everything here is an extern boundary.

#include "../ds/dsTSTRING.h"
#include "../fio/fioCHUNK.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dSPL.h"
#include "../anim/animTRS.h"
#include "../obj/objOBJ.h"
#include "../anim/animSEQ.h"

struct fioFILE;

// ws-engine fio cache: opens chunked resource files by (dir, name, ext). boundary.
struct fioCACHE {
    // 0x??? — open "<dir>/<name>.<ext>"; returns nullptr if absent.
    fioFILE *OpenFile(const char *dir, const char *name, const char *ext, unsigned int mode);
    void     CloseFile(fioFILE *file);
};
extern fioCACHE fioCache;

// fioFILE chunk/data readers used by mdlLIP_SYNCER::Start (declared here to avoid pulling the full
// fioFILE surface; the real methods live on fioFILE). boundary.
extern bool     fioFILE_ReadNextChunk(fioFILE *file, fioCHUNK *chunk);
extern void     fioFILE_ReadData(fioFILE *file, void *dst, int size, int reorderGain);
extern m3dSPL  *fioFILE_ReadSpline(fioFILE *file);

// anim: extract objOBJ's transform at `frame` of `seq` into `out`. boundary.
extern void animExtractObjAnim(objOBJ *obj, animSEQ *seq, float frame, animTRS *out);

// lipsync config namespace (globals set at init). boundary.
namespace lipsync {
    extern dsTSTRING<char> vo_path;              // voice-over spline directory
    extern dsTSTRING<char> ex_path;              // exclamation spline directory
    extern float           __maxLipSyncBlendTime; // ease-in/out clamp ceiling
    extern int             __dbgPhonemeId;        // debug: force a single phoneme (-1 = off)
}

// zero vector constant. boundary (also declared in anim/m3d boundary headers).
extern const m3dV m3dVZero;

// CRT/log helper. boundary.
extern void apLogErr(const char *fmt, ...);
