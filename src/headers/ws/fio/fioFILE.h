#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "FIO_OPEN.h"
#include "fioCHUNK_DESCR.h"
struct fioCHUNK; struct m3dSPL; /* fwd: ReadNextChunk/ReadChunk/ReadSpline params+return */
#include "fioFILE_BUFFER.h"
// ws-engine fio: abstract stream base class. DB-verified layout (types_members fioFILE):
//   __vftable@0, offset@4, streamSize@8, nDescr@12, nDescrAlloc@16, descrList@20,
//   pBuffer@24, state@28 — size 32.
//
// The slots below reproduce fioFILE_vtbl (types_members) IN ORDER so the
// concrete vtable layout matches: dtor, IsReadyToWrite, Term, IsEOF, SetBuffer,
// GetStreamName, DoSeek, DoReadData, DoWriteData, DoTruncate. Only Term is reversed
// in this batch (base impl at 0x825AF9A8); the rest are same-class boundaries.
// Virtual dispatch is through the explicit __vftable member (repo convention).

struct fioFILE_vtbl; // boundary — fio subsystem vtable (slot list mirrored below)

typedef struct fioFILE {
    fioFILE_vtbl         *__vftable;   // 0x00
    unsigned int          offset;      // 0x04 current stream position
    unsigned int          streamSize;  // 0x08 highest byte written / stream length
    int                   nDescr;      // 0x0C live chunk-descriptor count
    int                   nDescrAlloc; // 0x10 allocated chunk-descriptor slots
    fioCHUNK_DESCR       *descrList;   // 0x14 chunk-descriptor array
    fioFILE_BUFFER       *pBuffer;     // 0x18 active read/write buffer (null = unbuffered)
    dsFLAGS<FIO_OPEN,int> state;       // 0x1C open-mode / status flags

    // --- vtable (declaration order == slot order; all virtual via __vftable) ---
    ~fioFILE();                              // slot 0  boundary
    int IsReadyToWrite(int amount);          // slot 1  boundary
    int Term();                              // slot 2  0x825AF9A8 (reversed)
    int IsEOF();                             // slot 3  boundary
    void SetBuffer(int size);                // slot 4  boundary
    const char *GetStreamName();             // slot 5  boundary
    void DoSeek(unsigned int pos);           // slot 6  boundary
    void DoReadData(void *dst, int size);    // slot 7  boundary
    void DoWriteData(const void *src, int size); // slot 8  boundary
    void DoTruncate();                        // slot 9  boundary

    // --- non-virtual members reversed in this batch ---
    void Write(const char *s);                       // 0x825AF580
    void Write(const dsTSTRING<char> &str);          // 0x825AFD18
    void Read(char *str, int maxLen);                // 0x825ACC28
    void WriteData(const void *data, int size, int reorderGain); // 0x825AF408
    // 0x825ACF48 (?ReadData@fioFILE@@QAAXPAXHH@Z) — read `size` bytes into `data`, byte-reordering
    // in groups of `reorderGain` bytes (1 = no reorder). Thin wrapper over ReadDataImpl. boundary
    // (not decompiled by the HALO_SOUND_LIST/HALO_CHANNEL cluster; only called by it).
    void ReadData(void *data, int size, int reorderGain);
    // 0x825AF938 (?ReadLine@fioFILE@@QAA_NAAV?$dsTSTRING@D@@D@Z) — read one line (terminated by
    // `eoln`) into `*res`; returns false at EOF. boundary.
    bool ReadLine(dsTSTRING<char> *res, char eoln);

    // 0x8252C128 (?WriteChunk@fioFILE@@QAAXGPAVfioCHUNK@@@Z) — open a new stream chunk with id `id`,
    // recording its descriptor into *pChnk.  boundary — body external to this batch.
    void WriteChunk(unsigned short id, fioCHUNK *pChnk);
    // 0x8252C230 (?WriteLastChunk@fioFILE@@QAAXXZ) — close the current (last) chunk.  boundary.
    void WriteLastChunk();

    // 0x825ADCE8 — advance to the next chunk; returns 1 while more chunks remain,
    // 0 at end-of-file or on the error/eof state flags (bit 6 / bit 7). boundary ReadChunk below.
    int ReadNextChunk(fioCHUNK *pChnk);
    // 0x825AD??? (?ReadChunk@fioFILE@@...) — parse one chunk header into *pChnk; returns 1 at EOF. boundary.
    int ReadChunk(fioCHUNK *pChnk);
    // 0x825ADF58 — read a spline: a 4-byte tag selects the current (m3dSPL::Read) or
    // legacy (m3dSPL::ReadOld) on-disk format.
    m3dSPL *ReadSpline();

protected:
    // 0x825AC7F8 — buffered/unbuffered read core (IAA mangle == protected).
    void ReadDataImpl(void *data, int size, int reorderGain);
} fioFILE;
