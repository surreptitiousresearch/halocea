#pragma once
// HCEX sound bridge: per-Blam-tag play-count/category tracker, pooled via
// ds::NODE_POOL<TAG_INFO,TAG_INFO_POLICY> (HALO_SOUND_SYSTEM::tagInfoPool). Used by
// HALO_SOUND_SYSTEM::StealOldChannel to pick which currently-playing sound to cut when a new
// one of a higher-priority category needs a channel.
// DB-verified layout (types_members TAG_INFO): name@0 (const char*), count@4 (int),
// state@8 (dsFLAGS<TAG_INFO_STATE,int>) — size 12.

#include "../ws/ds/dsFLAGS.h"
#include "../ws/ds/dsVECTOR.h"

// types_enum_values TAG_INFO_STATE.
enum TAG_INFO_STATE {
    TAG_INFO_WEAPON = 1,
    TAG_INFO_AMBIENT = 2,
    TAG_INFO_DIALOG = 4,
    TAG_INFO_MUSIC = 8,
};

typedef struct TAG_INFO {
    const char                     *name;  // 0x00
    int                              count;  // 0x04 number of currently-playing sounds for this tag
    dsFLAGS<TAG_INFO_STATE, int>    state;   // 0x08

    TAG_INFO(); // ??0TAG_INFO@@QAA@XZ -- boundary, referenced (Alloc'd) by the HALO_PERM_SOUND
                // cluster's Init/Stop but the ctor body itself is a sibling-owned pool node
                // lifecycle detail owned by a sibling batch.
} TAG_INFO;

// ds::NODE_POOL<TAG_INFO,TAG_INFO_POLICY> — DB-verified layout: usedNodes@0
// (dsVECTOR<TAG_INFO*,8>), policy@20 (TAG_INFO_POLICY<TAG_INFO>) — size 40.
template<class T> struct TAG_INFO_POLICY; // fwd, defined below

namespace ds {

template<class T, template<class> class POLICY>
struct NODE_POOL {
    dsVECTOR<T *, 8>   usedNodes; // 0x00 nodes currently checked out
    POLICY<T>          policy;    // 0x14 free-list + allocation policy

    void Free(); // boundary — sibling type (frees every pooled node)
};

} // namespace ds

// TAG_INFO_POLICY<TAG_INFO> — DB-verified layout: freeNodes@0 (dsVECTOR<TAG_INFO*,8>) — size 20.
template<class T>
struct TAG_INFO_POLICY {
    dsVECTOR<T *, 8>   freeNodes; // 0x00

    void Free(); // boundary — sibling type

    // Checks out a node for `name` (reusing a freeNodes entry or allocating one), pushing it
    // onto `used`.     // batch; declared here because HALO_PERM_SOUND::Init calls it directly.
    T *Alloc(const char *name, dsVECTOR<T *, 8> &used); // ?Alloc@?$TAG_INFO_POLICY@UTAG_INFO@@@@QAAPAUTAG_INFO@@PBDAAV?$dsVECTOR@PAUTAG_INFO@@$07@@@Z

    // Returns `ptr` to the pool (removing it from `used`). Same boundary note as Alloc();
    // declared here because HALO_PERM_SOUND::Stop calls it directly.
    void Delete(T *ptr, dsVECTOR<T *, 8> &used); // ?Delete@?$TAG_INFO_POLICY@UTAG_INFO@@@@QAAXPAUTAG_INFO@@AAV?$dsVECTOR@PAUTAG_INFO@@$07@@@Z
};
