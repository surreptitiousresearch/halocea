#pragma once
#include "../ws/inst/instMANAGER_HANDLER.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsPAIR.h"
#include "../ws/scn/scnINST_CREATE_DATA.h"
#include "../ws/ds/dsTSTRING.h"
struct scnINST_CREATE_DATA;

// HCEX bridge: instMANAGER_HANDLER that intercepts scenery instance creation to route Halo-model
// scenery through the Blam bridge instead of the ws-engine's native scenery path. DB-verified
// layout (types_members hcexHALO_CUSTOM_OBJ_LIST): base instMANAGER_HANDLER @0,
// cdList@4 (dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>) — size 24.
// Slot order matches the corrected instMANAGER_HANDLER_vtbl layout (IsInstManagable@0, dtor@4) —
// this class overrides IsInstManagable and inherits the dtor slot's position.
typedef struct hcexHALO_CUSTOM_OBJ_LIST_vtbl {
    bool (*IsInstManagable)(struct hcexHALO_CUSTOM_OBJ_LIST *self, scnINST_CREATE_DATA *data); // 0x00
    void (*dtr_hcexHALO_CUSTOM_OBJ_LIST)(struct hcexHALO_CUSTOM_OBJ_LIST *self, int freeMemory); // 0x04
} hcexHALO_CUSTOM_OBJ_LIST_vtbl;

struct hcexHALO_CUSTOM_OBJ_LIST : instMANAGER_HANDLER {
    static hcexHALO_CUSTOM_OBJ_LIST_vtbl vftable; // `hcexHALO_CUSTOM_OBJ_LIST::`vftable'' — boundary
    dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8> cdList; // 0x04 sceneryIdx/typeId -> pending create data

    // 0x823D952C — register the vftable, default-construct cdList (adopting the
    // "halo_main.cpp":222 allocation call-site cookie).
    hcexHALO_CUSTOM_OBJ_LIST();

    // 0x823D97F0 — tear down cdList, reset __vftable to the instMANAGER_HANDLER base slot.
    ~hcexHALO_CUSTOM_OBJ_LIST() override;

    // 0x823DCBA8 — compiler-generated vector deleting destructor (`??_E` mangle): runs the
    // destructor body, then conditionally operator-deletes `this` when bit 0 of `deleteFlags` is set.
    hcexHALO_CUSTOM_OBJ_LIST *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823D97D4 — drop every pending create-data entry.
    void Clear();

    // 0x823DCAF4 — look up the pending create-data entry keyed by (sceneryIdx,typeId) and copy out
    // its template/class/instance names. Returns false (and leaves the out-params untouched) when
    // no entry is sorted-found for the key.
    bool GetClsTplName(int sceneryIdx, int typeId, dsTSTRING<char> &clsName,
                        dsTSTRING<char> &tplName, dsTSTRING<char> &instName);

    // 0x823E5578 — decide whether a scene-instance creation should be intercepted by this handler:
    // true for breakable surfaces; for "h<sceneryIdx>_<typeId>|h..." named halo-model instances,
    // records the create data into cdList (sorted, duplicate-ignore) and returns true; otherwise
    // returns whether the instance's class name is "halo_no_model".
    bool IsInstManagable(scnINST_CREATE_DATA *pInstCD); // base slot is a vtable fn-ptr, not a C++ virtual: no 'override'

    // 0x823D95B8 — true when the instance's class name is exactly "halo_no_model" (used by callers
    // that need the plain skip check without the side-effecting IsInstManagable path).
    bool IsSkipCreation(scnINST_CREATE_DATA *pInstCD);
};
