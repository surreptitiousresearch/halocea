#pragma once
#include "../ds/dsEVENT_HANDLER.h"
#include "../ds/dsVECTOR.h"
#include "../ds/REF_TYPE.h"
#include "../ssl/sslOBJ_REF.h"
// ws-engine gs: the user manager singleton — tracks the local user slots (one
// ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> profile per slot) and publishes which of them is the
// primary/secondary local player. DB-verified layout (types_members gsUSER_MNG, size 40):
// eventDispatcher@0 (gsUSER_MNG::EVENT_DISP_gsUSER_MNG, 8B), sslObject@8 (sslOBJ_REF),
// users@0x0C (dsVECTOR<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>,8>, 20B), primaryUserIdx@0x20 (int),
// secondaryUserIdx@0x24 (int).

struct dsVECTOR_PARAM_LIST; // boundary — parameter-list vector class (users[] element payload)

typedef struct gsUSER_MNG {
    // Nested dsEVENT_HANDLER subscriber (types_members gsUSER_MNG::EVENT_DISP_gsUSER_MNG:
    // dsEVENT_HANDLER base@0, pObj@4 — size 8).
    struct EVENT_DISP_gsUSER_MNG : public dsEVENT_HANDLER {
        gsUSER_MNG *pObj; // 0x04 back-pointer to the owning manager
    };

    EVENT_DISP_gsUSER_MNG                               eventDispatcher;  // 0x00
    sslOBJ_REF                                          sslObject;        // 0x08
    dsVECTOR<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>, 8> users;            // 0x0C
    int                                                 primaryUserIdx;   // 0x20
    int                                                 secondaryUserIdx; // 0x24 — size 40

    // 0x823C1370 (?PrimaryUserIdx@gsUSER_MNG@@QBAHXZ) — the primary local user's slot index. const (QBA).
    int PrimaryUserIdx() const;
    // 0x82630ED0 (?SecondaryUserIdx@gsUSER_MNG@@QAAHXZ) — the secondary local user's slot index.
    // Non-const (QAA) per the DB mangle — reads/maintains SelectSecondaryUser's live state.
    int SecondaryUserIdx();
} gsUSER_MNG;

// ?gsUserMng@@3PAVgsUSER_MNG@@A — mangled (C++ linkage); no extern "C" here on purpose.
extern gsUSER_MNG *gsUserMng;
