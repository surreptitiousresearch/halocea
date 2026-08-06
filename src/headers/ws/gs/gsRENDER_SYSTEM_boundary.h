#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ap/apCOUNTER_TIME_RECUR.h"
// Boundary declarations for gsRENDER_SYSTEM__SwapBuffers.cpp / gsRENDER_SYSTEM___SaveScreenShot.cpp:
// the large self-contained ws-engine driver/manager classes and OS helpers these two functions
// call into but do not themselves reverse. Per-consumer minimal model, at DB-verified offsets
// where read directly (see e.g. hcex_render_present_boundary.h for this corpus's established
// convention for the same classes).

// --- canonical DB-verified full layouts (the former per-consumer partial slices were
// consolidated): vid config/driver, rend driver, d3d driver ---
#include "../../vidDRIVER.h"     /* vidDRIVER (curConfig@0x1C4, ...) + vidCONFIG (GetVSync) */
#include "../rend/rendDRIVER.h"  /* rendDRIVER (EndFrame is virtual dispatch) */
#include "../../d3d_driver.h"    /* d3dDRIVER (ConvertPWL2ProperGamma) */
#include "../fio/fioFILE_DISK.h" /* fioFILE_DISK (gsAPP_SYSTEM::anitecLog) */

extern vidDRIVER  *vidDriver;
extern rendDRIVER *rendDrv;
extern d3dDRIVER  *d3dDriver;

// --- picture/texture-format subsystem (pct*) — screenshot capture buffer ---
// Use canonical pctHEADER (was a local redefinition here → C2011 when a TU also pulled the
// canonical header).
#include "../../pctHEADER.h"

/* DB anonymous per-channel unions ($-names kept verbatim; $ in identifiers is an MSVC
 * extension). */
typedef union _BA6C9019639D1FC0F8714ED004CDCE2D { unsigned char r; unsigned char du; } _BA6C9019639D1FC0F8714ED004CDCE2D;
typedef union _61DE270AFB62A4230A98352E006E278D { unsigned char g; unsigned char dv; } _61DE270AFB62A4230A98352E006E278D;
typedef union _F828A1381875C7AF688A36EC0FF1F720 { unsigned char b; unsigned char i; } _F828A1381875C7AF688A36EC0FF1F720;

// DB-verified layout (types_members pctPIXEL) — size 4. NOTE: channel order is a,r,g,b
// (the former boundary spelling's b,g,r,a order was wrong).
typedef struct pctPIXEL {
    unsigned char a;                               /* 0x00 */
    union _BA6C9019639D1FC0F8714ED004CDCE2D ___u1; /* 0x01 r / du */
    union _61DE270AFB62A4230A98352E006E278D ___u2; /* 0x02 g / dv */
    union _F828A1381875C7AF688A36EC0FF1F720 ___u3; /* 0x03 b / i */
} pctPIXEL;

struct pctPICTURE; // boundary — opaque; only used by pointer, per pct_boundary.h's convention

extern pctPICTURE *pctCreate(const pctHEADER *hdr);
extern void        pctDestroy(pctPICTURE *pic);
extern void        pctGetPixel(pctPICTURE *pic, int x, int y, int mip, pctPIXEL *outPixel, int unused0, int unused1);
extern void        pctSetPixel(pctPICTURE *pic, int x, int y, int mip, const pctPIXEL *pixel, int unused0, int unused1);
extern int         pctSaveBMP(const char *path, pctPICTURE *pic); // DB: int return (was bool)

// --- app system (anitec automated-screenshot mode) ---
// DB-verified layout (types_members gsAPP_SYSTEM) — size 352.
struct gsAPP_SYSTEM_vtbl; // boundary
typedef struct gsAPP_SYSTEM {
    gsAPP_SYSTEM_vtbl           *__vftable;       // 0x00
    dsTSTRING<char>              cmdLine;         // 0x04
    dsVECTOR<dsTSTRING<char>, 8> anitecNames;     // 0x08
    int                          anitecShotIdx;   // 0x1C
    bool                         anitecSoundOnly; // 0x20
    unsigned char                _pad21[3];       // 0x21 db-verified padding
    fioFILE_DISK                 anitecLog;       // 0x24 (316B)
} gsAPP_SYSTEM;

extern gsAPP_SYSTEM *gsAppSystem;

// --- misc globals / OS helpers ---
extern "C" apCOUNTER_TIME_RECUR gsRendCnt_Swap;
extern "C" bool                 FORCE_SHOT;
extern dsTSTRING<char>      gsLevelName;

extern "C" void UpdateGPUCounters();
extern bool osFileIsExist(const char *path);
extern void osFileMakeDir(const char *path);
// DB return type is non-const dsTSTRING<char>* (?dsSPrintf@@YA?AV?$dsTSTRING@D@@PBDZZ); const removed
// to match the canonical decl in hcex_ds_boundary.h and avoid a C2556 differ-by-return-type clash.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

// libc time helpers (Xbox 360 CRT 64-bit time_t variants).
extern "C" {
    long long   _time64(long long *t);
    struct tm  *localtime64(const long long *t);
}
