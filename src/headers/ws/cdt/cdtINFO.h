#pragma once
#include "../m3d/m3dV.h"
#include "../ap/apSTATE_T.h"
// ws-engine cdt (collision-detection): a collision-query result record (hit point, distance,
// face/bone/capsule ids, owning object). DB-verified layout (types_members cdtINFO):
//   bitfields@0 (6 reserved + SetVClosestCalled + SetDistMinCalled, 1 byte), 3 opaque 4-byte
//   fields@4/8/12 (face/bone/capsule identifying data unions, named per the DB in
//   this batch), vClosest@16 (m3dV, 12B), distMin@28 (float), state@32 (apSTATE_T<unsigned long>)
//   — size 36.

struct animINST; // ../anim/animINST.h — pointer only
struct m3dPOLY;  // ../m3d/m3dPOLY.h — pointer only

/* DB anonymous unions ($-names kept verbatim; $ in identifiers is an MSVC extension). */
typedef union _3AD1313BC80B76D2FE057569F6656FF0 {
    void     *pData; /* 0x00 */
    animINST *pInst; /* 0x00 */
} _3AD1313BC80B76D2FE057569F6656FF0;
typedef union _F9E0F739CD022C562857317C7468BCE0 {
    int id;    /* 0x00 */
    int idObj; /* 0x00 */
} _F9E0F739CD022C562857317C7468BCE0;
typedef union _1C4B282FCB9CE24012D58874BBA83F3F {
    int id2;    /* 0x00 */
    int idFace; /* 0x00 */
} _1C4B282FCB9CE24012D58874BBA83F3F;

typedef struct cdtINFO {
    unsigned char reserved_bits : 6;      // 0x00 bits 0-5, unused by any reversed method
    unsigned char SetVClosestCalled : 1;  // 0x00 bit 6
    unsigned char SetDistMinCalled : 1;   // 0x00 bit 7
    unsigned char _pad1[3];               // 0x01 padding to the next 4-byte field
    union _3AD1313BC80B76D2FE057569F6656FF0 ___u2; // 0x04 pData / pInst (DB anonymous union)
    union _F9E0F739CD022C562857317C7468BCE0 ___u3; // 0x08 id / idObj (DB anonymous union)
    union _1C4B282FCB9CE24012D58874BBA83F3F ___u4; // 0x0C id2 / idFace (DB anonymous union)
    m3dV          vClosest;               // 0x10 closest hit point
    float         distMin;                // 0x1C closest hit distance
    apSTATE_T<unsigned long> state;       // 0x20

    // 0x82C16020 — zero every field (state plus the 32 bytes that follow it, i.e. the whole
    // record: bitfields/_u2/_u3/_u4/vClosest/distMin get zeroed too via the same word-loop).
    // DEVIATION: the raw decompile writes `this->state.state = 0` once, then a 9-word loop
    // starting at `this-4`'s first pre-increment (offset 0) through offset 32 inclusive — i.e.
    // it zeroes the entire 36-byte object (bitfields..state), matching a plain memset(this,0,
    // sizeof(*this)) collapsed here to per-field zero-inits for readability.
    cdtINFO();

    // 0x82743A40 — compute the hit face normal (norm) and, if poly!=null, its polygon. boundary.
    int GetFacePolyNormal(m3dPOLY *poly, m3dV *norm) const;
    // 0x82744288 — face normal only (poly==null forwarder onto GetFacePolyNormal).
    int GetFaceNormal(m3dV *norm) const;

    void SetVClosest(const m3dV &v); // 0x82C1... — sets vClosest + SetVClosestCalled
    void SetDistMin(float d);        // sets distMin + SetDistMinCalled
    void Clear();                    // reset to default (re-zero)

    // 0x82C16078 — valid flag: bit 1 of state (== SetVClosestCalled recorded in state word).
    unsigned int IsValid() const;    // 0x82C16078
    // 0x82C16088 — identical read of state bit 1 (separate accessor name in source).
    unsigned int Checker() const;    // 0x82C16088
    // 0x82C16098 — accessor for the closest hit point.
    const m3dV *GetVClos() const;    // 0x82C16098
} cdtINFO;
