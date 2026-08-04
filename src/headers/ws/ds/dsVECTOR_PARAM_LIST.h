#pragma once
#include "dsPARAM_LIST.h"
#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "dsSTRID.h"
// ws-engine ds: an owning parameter list — a dsVECTOR<dsPARAM> whose count/pointer view is
// mirrored into the dsPARAM_LIST base after each mutation.
// DB-verified layout (types_members dsVECTOR_PARAM_LIST):
//   <dsPARAM_LIST base>@0 (8 bytes), storage@8 (dsVECTOR<dsPARAM,8>, 20 bytes).

struct dsVECTOR_PARAM_LIST : dsPARAM_LIST {
    dsVECTOR<dsPARAM, 8> storage; // 0x08 backing element storage

    // 0x823E32A0 (int) / 0x823E3330 (double) — append a parameter named `id` holding `val`,
    // then refresh the dsPARAM_LIST base view (length/list) from the backing store.
    template<class T>
    void Add(dsSTRID id, const T &val);

    // 0x823E30B0 (int) — set the parameter named `id` to `val`: overwrite the existing entry if
    // present, otherwise append a fresh one (refreshing the base view). Upsert form of Add.
    template<class T>
    void Set(dsSTRID id, const T &val);

    // 0x82618530 — set the parameter named `param.id` from `param`: overwrite the existing entry
    // if present, otherwise append a copy (refreshing the base view).
    void Set(const dsPARAM &param);
};
