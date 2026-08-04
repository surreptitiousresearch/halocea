#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/bhvDSC_STORAGE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiPLANNER::RegisterBehaviorDescs() @ 0x8316D908  (virtual)
// Register every built-in AI behavior brand (the bvd* descriptor family) with the planner's
// behavior-descriptor store, each parented under the named base brand. Registration order and
// parentage are exactly as emitted by the original a8_planner.cpp.

// bvd* behavior-descriptor types — boundary (each is a dscDESC subclass registered by name).
struct bvdSIMPLE;   struct bvdMAJOR;      struct bvdDEAD;   struct bvdROAM;
struct bvdPATH;     struct bvdHO_PATH;    struct bvdHO_SPEC_MOVE; struct bvdAVOID;
struct bvdSTAY;     struct bvdSHOOT;      struct bvdSNS;    struct bvdDNS;
struct bvdSNIPE;    struct bvdIDLE;       struct bvdHO;     struct bvdCHASE;
struct bvdMAJOR_SMART; struct bvdSMART;   struct bvdIDLE_SMART; struct bvdTURRET;
struct bvdHLB;      struct bvdMELEE;

template<class DESC>
static void RegisterBhv(bhvDSC_STORAGE *store, const char *name, const char *parent)
{
    dsTSTRING<char> brandName, parentName;
    brandName.UnsafeInit(name, -1, 0);
    parentName.UnsafeInit(parent, -1, 0);
    store->RegisterBrand<DESC>(brandName, parentName);
}

void aiPLANNER::RegisterBehaviorDescs()
{
    bhvDSC_STORAGE *store = this->bhvDStore;

    RegisterBhv<bvdSIMPLE>       (store, "d_simple",       "null");
    RegisterBhv<bvdMAJOR>        (store, "d_major",        "d_simple");
    RegisterBhv<bvdDEAD>         (store, "d_dead",         "d_simple");
    RegisterBhv<bvdROAM>         (store, "d_roam",         "d_simple");
    RegisterBhv<bvdPATH>         (store, "d_path",         "d_simple");
    RegisterBhv<bvdHO_PATH>      (store, "d_ho_path",      "d_simple");
    RegisterBhv<bvdHO_SPEC_MOVE> (store, "d_ho_spec_move", "d_simple");
    RegisterBhv<bvdAVOID>        (store, "d_avoid",        "d_simple");
    RegisterBhv<bvdSTAY>         (store, "d_stay",         "d_simple");
    RegisterBhv<bvdSHOOT>        (store, "d_shoot",        "d_simple");
    RegisterBhv<bvdSNS>          (store, "d_sns",          "d_stay");
    RegisterBhv<bvdDNS>          (store, "d_dns",          "d_simple");
    RegisterBhv<bvdSNIPE>        (store, "d_snipe",        "d_stay");
    RegisterBhv<bvdIDLE>         (store, "d_idle",         "d_stay");
    RegisterBhv<bvdHO>           (store, "d_ho",           "d_simple");
    RegisterBhv<bvdCHASE>        (store, "d_chase",        "d_simple");
    RegisterBhv<bvdMAJOR_SMART>  (store, "d_major_smart",  "d_simple");
    RegisterBhv<bvdSMART>        (store, "d_smart",        "d_simple");
    RegisterBhv<bvdIDLE_SMART>   (store, "d_idle_smart",   "d_smart");
    RegisterBhv<bvdTURRET>       (store, "d_turret",       "d_smart");
    RegisterBhv<bvdHLB>          (store, "d_hlb",          "d_simple");
    RegisterBhv<bvdMELEE>        (store, "d_melee",        "d_simple");
}
