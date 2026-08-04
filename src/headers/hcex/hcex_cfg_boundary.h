#pragma once
/* Boundary declarations for the ws-engine per-user config system (apCONFIG / gsUSER_CFG_MNG /
 * ds_data::REF_TYPE<apCONFIG>) touched by hcex_get_skull_state to read a skull's persisted
 * on/off value out of the active user's saved config ("GameOpt.Skulls.skullNN"). These are large,
 * self-contained engine classes reached via only two or three method calls here, so only the
 * touched surface is modeled; the rest is an opaque boundary.
 *
 * ds_data::REF_TYPE<apCONFIG> / REF_TYPE_DATA<apCONFIG> layouts are DB-verified in full
 * (types_members): REF_TYPE<apCONFIG> is a 4-byte smart-pointer wrapper (m_pData), and
 * REF_TYPE_DATA<apCONFIG> is m_pT@0x00 (apCONFIG*), m_iRefCount@0x04, m_op@0x08 (12 bytes).
 * apCONFIG itself (116 bytes) is left opaque — only reached through GetValue<bool>. gsUSER_CFG_MNG
 * (40 bytes) is left opaque apart from its Get() factory. gsUSER_MNG is modeled only up to
 * primaryUserIdx@0x20 (types_members gsUSER_MNG), the one field hcex_get_skull_state reads. */

typedef struct apCONFIG apCONFIG;                 /* full layout: ws/ap/apCONFIG.h — boundary here */
typedef struct gsUSER_CFG_MNG gsUSER_CFG_MNG;      /* full layout: hcex/engine_control boundary — opaque here */

/* ds_data::REF_TYPE<apCONFIG> / REF_TYPE_DATA<apCONFIG> — the generic DB-verified templates. */
#include "../ws/ds/REF_TYPE.h"

namespace ds_data
{
    typedef REF_TYPE_DATA<apCONFIG> REF_TYPE_DATA_apCONFIG; /* 12 bytes */
    typedef REF_TYPE<apCONFIG>      REF_TYPE_apCONFIG;      /* 4 bytes */
}

/* Full DB layout (types_members gsUSER_MNG, size 40); the dispatcher/handle members are spelled
 * with the ws-engine types. */
#include "../ws/ssl/sslOBJ_REF.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsEVENT_MGR.h"
struct dsVECTOR_PARAM_LIST; /* boundary — parameter-list vector class */

typedef struct gsUSER_MNG
{
    /* Nested dsEVENT_HANDLER subscriber (types_members gsUSER_MNG::EVENT_DISP_gsUSER_MNG:
     * dsEVENT_HANDLER base@0, pObj@4 — size 8). */
    struct EVENT_DISP_gsUSER_MNG : public dsEVENT_HANDLER {
        gsUSER_MNG *pObj; /* 0x04 */
    };

    EVENT_DISP_gsUSER_MNG                               eventDispatcher;  /* 0x00 */
    sslOBJ_REF                                          sslObject;        /* 0x08 */
    dsVECTOR<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>, 8> users;            /* 0x0C */
    int           primaryUserIdx; /* 0x20 */
    int           secondaryUserIdx; /* 0x24 */
} gsUSER_MNG;                     /* 40 bytes */

extern gsUSER_CFG_MNG *gsUserCfgMng;
extern gsUSER_MNG      *gsUserMng;

/* 0x823C1478-adjacent boundary calls (free-function thiscall form). */
extern ds_data::REF_TYPE_apCONFIG gsUSER_CFG_MNG_Get(gsUSER_CFG_MNG *self, int userIdx);   /* ?Get@gsUSER_CFG_MNG@@QAA?AV?$REF_TYPE@VapCONFIG@@@ds_data@@H@Z */
extern void ds_data_REF_TYPE_apCONFIG_dtor(ds_data::REF_TYPE_apCONFIG *self);               /* ??1?$REF_TYPE@VapCONFIG@@@ds_data@@QAA@XZ */
extern bool apCONFIG_GetValue_bool(apCONFIG *self, const char *name, bool *out_value);       /* ??$GetValue@_N@apCONFIG@@QBA_NPBDAA_N@Z */
