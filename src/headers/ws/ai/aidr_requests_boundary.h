#pragma once
#include "aidr_DR.h"
#include "aiCDT_REQ_PARAMS.h"

// ws-engine ai08 (a8_def_cdt_sys) BOUNDARY: the concrete aidr::DR request subclasses produced by
// aiDEF_CDT_SYS::Post*Request. Each derives aidr::DR (16 bytes) and carries a byte-accurate opaque
// tail sized to the DB `operator new` allocation for that subclass — their interiors are the next
// frontier. Every subclass overrides a secondary request-parameter interface whose first virtual
// (aidr::DR_vtbl-sized entry [1] slot 0) copies the caller's params into the request; that override
// is spelled here as PostParams(). Constructors are defined in the boundary TU.

struct aiBRAIN;

// aiREFINE_GEO_MOTION — reach/motion geometric refine filter. DB-verified (types_members
// aiREFINE_GEO_MOTION, size 32): an aiREFINE_GEO_COMMON base (= cdtREFINE) with no additional
// members (own vtable/ctor only).
#include "aiREFINE_GEO_COMMON.h"
struct aiREFINE_GEO_MOTION : aiREFINE_GEO_COMMON {
    // ??0aiREFINE_GEO_MOTION@@QAA@PAVaiBRAIN@@@Z — boundary.
    aiREFINE_GEO_MOTION(aiBRAIN *sender);
    // ??1aiREFINE_GEO_MOTION@@UAA@XZ — boundary.
    ~aiREFINE_GEO_MOTION();
    // ?IsInclInst@aiREFINE_GEO_MOTION@@UAAHPAVanimINST@@@Z — boundary (virtual via cdtREFINE vtbl).
    int IsInclInst(animINST *inst);
};

namespace aidr {

struct DRREACH_SIMPLE : DR {                 // DB alloc size 0x60
    DRREACH_SIMPLE(float ttl, const aiREFINE_GEO_MOTION &motion);
    void PostParams(const aiREACH_REQ_PARAMS &params);
    unsigned char _tail[0x60 - 0x10];
};

struct DRVIS_CIRC : DR {                     // DB alloc size 0x14C
    DRVIS_CIRC(float ttl);
    void PostParams(const aiVIS_REQ_PARAMS &params);
    unsigned char _tail[0x14C - 0x10];
};

struct DRVIS_SLICED : DR {                   // DB alloc size 0x164
    DRVIS_SLICED(float ttl);
    void PostParams(const aiVIS_REQ_PARAMS &params);
    unsigned char _tail[0x164 - 0x10];
};

struct DRVIS_LONG : DR {                     // DB alloc size 0x1A4
    DRVIS_LONG(float ttl);
    void PostParams(const aiVIS_REQ_PARAMS &params);
    unsigned char _tail[0x1A4 - 0x10];
};

struct DRFE : DR {                           // DB alloc size 0x28
    DRFE(float ttl);
    void PostParams(const aiFE_REQ_PARAMS &params);
    unsigned char _tail[0x28 - 0x10];
};

struct DRTRAJ_LINEAR : DR {                  // DB alloc size 0xC4
    DRTRAJ_LINEAR(float ttl, const aiREFINE_TRAJ_NEAR &near, const aiREFINE_TRAJ_FAR &far);
    void PostParams(const aiTRAJ_REQ_PARAMS &params);
    unsigned char _tail[0xC4 - 0x10];
};

struct DRTRAJ_PARABOLIC : DR {               // DB alloc size 0x1F0
    DRTRAJ_PARABOLIC(float ttl, const aiREFINE_TRAJ_NEAR &near);
    void PostParams(const aiTRAJ_REQ_PARAMS &params);
    unsigned char _tail[0x1F0 - 0x10];
};

struct DRDNS_PNT : DR {                      // DB alloc size 0xEC
    DRDNS_PNT(float ttl);
    void PostParams(const aiDNS_PNT_REQ_PARAMS &params);
    unsigned char _tail[0xEC - 0x10];
};

} // namespace aidr
