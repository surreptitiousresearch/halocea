/* hcex_get_skull_state @0x823D4EC8 — read one HCEX skull's persisted on/off value out of the
 * primary user's saved config ("GameOpt.Skulls.skullNN"), OR it with the live debug-console
 * override (`dbg_var`, passed in by each dbg_skullNN toggle's caller), and if either is set, bump
 * the appropriate gold/silver skull counter (skulls 0 == gold, 1+ == silver) and report "on".
 *
 * apCONFIG / gsUSER_CFG_MNG / ds_data::REF_TYPE<apCONFIG> are ws-engine config-system boundaries
 * (see hcex_cfg_boundary.h) reached through only Get()/GetValue<bool>() here.
 *
 * DEVIATION: the decompiler mistyped the on-stack REF_TYPE<apCONFIG> local as `gsUSER_CFG_MNG
 * v7[3]` (coincidental size overlap with the snprintf buffer) and rendered the GetValue<bool>
 * `this` argument as `v7[0].dtr_gsUSER_CFG_MNG` — that field is exactly
 * REF_TYPE<apCONFIG>::m_pData->m_pT (the live apCONFIG*), per types_members
 * REF_TYPE_DATA<apCONFIG>. Rewritten below using the real ds_data::REF_TYPE<apCONFIG> layout. */

#include "../headers/hcex/hcex_cfg_boundary.h"

extern int _snprintf_0(char *buf, unsigned int size, const char *fmt, ...);

extern int hcex_gold_skulls_count;
extern int hcex_silver_skulls_count;

int hcex_get_skull_state(int skull_index, bool dbg_var)
{
    char cfg_key[128];
    _snprintf_0(cfg_key, 0x80, "GameOpt.Skulls.skull%02d", skull_index);

    ds_data::REF_TYPE_apCONFIG cfg_ref = gsUSER_CFG_MNG_Get(gsUserCfgMng, gsUserMng->primaryUserIdx);
    apCONFIG *cfg = cfg_ref.m_pData->m_pT;

    bool saved_value = false;
    bool found = apCONFIG_GetValue_bool(cfg, cfg_key, &saved_value);
    bool skull_on = (found && saved_value) || dbg_var;

    if ( skull_on )
    {
        if ( skull_index < 1 )
            ++hcex_gold_skulls_count;
        else
            ++hcex_silver_skulls_count;
    }

    ds_data_REF_TYPE_apCONFIG_dtor(&cfg_ref);
    return skull_on ? 1 : 0;
}
