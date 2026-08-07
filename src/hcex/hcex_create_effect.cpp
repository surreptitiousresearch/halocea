/* hcex_create_effect @0x823E5DF0 — Blam-to-ws bridge entry that spawns a named effect/sound at one or more
 * world points. Resolves the Blam effect name to a ws sfx brand (via the sorted hcex_effect_names table, or
 * hcex_conv_eff_name as a fallback), applies a handful of game-specific special cases, then either records
 * the effect for later processing (obj_follow == -1) or attaches it to an existing object.
 *
 * Special cases handled:
 *   - "sfx_slip_sand" underwater becomes "sfx_slip_vehicle_water".
 *   - For a first-person player view (plr_idx), the matching cyborg_fp model's foot/hand sml state is set.
 *   - Grunt Birthday Party skull: a grunt killed by a headshot plays "sfx_konfetti" + the "skull_laugh" sound.
 *   - Weapon / vehicle objects with their own sfx list get the sound routed through their entity sml state.
 *   - Unknown brands are recorded in hcexMissedSfx (and optionally hcexUsedSfx when the dump toggle is on).
 *
 * The ws template containers, sml/snd/ent systems are extern boundaries (see hcex_create_effect_boundary.h).
 *
 * NOTE: per the database, each location's pos is taken from `positions[i]` and vec from `vectors[i]`; the
 * decompiler expresses this through a (positions - vectors) byte offset added to the advancing `vectors`
 * cursor — reconstructed here as the plain indexed form.
 * NOTE: in the attached first-person/weapon/vehicle paths the original derives the entity sml state slot at
 * byte offset 0x24 of the entity (3 sml::STATE units); preserved as an explicit byte offset. */

#include "../headers/hcex/hcex_create_effect_boundary.h"
#include "../headers/damage_part.h"

static void tstring_release(dsTSTRING_flat *s)
{
    if ( s->pBuffer->refCount-- == 1 )
        dlFree(s->pBuffer);
}

/* inline strcmp the decompiler emits: returns 0 when the two C strings are equal */
static int cstr_cmp(const char *a, const char *b)
{
    int delta = 0;
    while ( 1 )
    {
        if ( !*a )
            break;
        delta = (unsigned char)*a - (unsigned char)*b;
        if ( delta )
            break;
        ++a;
        ++b;
    }
    return delta;
}

/* returns 0 when the ref-counted string equals literal `lit` */
static int tstring_equals(const dsTSTRING_BUF_HEADER<char> *s, const char *lit)
{
    return cstr_cmp(s->str, lit);
}

/* true when the C string `a` equals literal `b` (cstr_cmp returns 0 on equality) */
static bool cmp_name_equals(const char *a, const char *b)
{
    return cstr_cmp(a, b) == 0;
}

extern "C" void hcex_create_effect(const char *name, int obj_follow, int plr_idx, hcex_float3 *positions,
        hcex_float3 *vectors, const char **names, int npoints)
{
    static dsTSTRING_flat emptyStr;
    static int emptyStr_guard;

    int killed_body_part = hcex_last_killed_in_body_part;
    dsTSTRING_flat name_str;
    dsTSTRING_flat sfx_name;
    dsCMP cmp;
    dsTSTRING_BUF_HEADER<char> *name_buffer;
    char conv_buf[64];
    char inst_name[176];
    int found;

    hcex_last_killed_in_body_part = -1;
    if ( !dbg_hcex_create_effects.value )
        return;

    name_str.pBuffer = 0;
    dsTSTRING_UnsafeInit(&name_str, name, -1, 0);

    /* sfx_name starts as the shared empty string */
    sfx_name.pBuffer = 0;
    if ( (emptyStr_guard & 1) == 0 )
    {
        emptyStr.pBuffer = 0;
        emptyStr_guard |= 1;
        emptyStr.pBuffer = dsTSTRING_AllocBuffer(&emptyStr, 0, 0);
        /* original registers a dynamic atexit destructor for emptyStr here — boundary, not reversed */
    }
    sfx_name.pBuffer = emptyStr.pBuffer;
    ++emptyStr.pBuffer->refCount;

    cmp = dsCMP();
    found = dsVECTOR_PAIR_FindSorted(&hcex_effect_names, &name_str, &cmp);
    if ( found >= 0 )
    {
        dsPAIR_TSTR *pair = dsVECTOR_PAIR_index(&hcex_effect_names, found);
        dsTSTRING_assign(&sfx_name, &pair->val);
    }
    else
    {
        dsTSTRING_flat converted;
        hcex_conv_eff_name(name, conv_buf, 64);
        converted.pBuffer = 0;
        dsTSTRING_UnsafeInit(&converted, conv_buf, -1, 0);
        dsTSTRING_assign(&sfx_name, &converted);
        tstring_release(&converted);
    }

    name_buffer = name_str.pBuffer;

    if ( dbg_hcexDumpUsedSfxOn.value )
    {
        dsTSTRING_flat dump;
        int at;
        ++name_str.pBuffer->refCount;
        dump.pBuffer = name_str.pBuffer;
        dsTSTRING_Insert(&dump, name_str.pBuffer->strLen, " s3d name '", -1);
        at = dump.pBuffer->strLen;
        dsTSTRING_InsertRepeat(&dump, at, sfx_name.pBuffer->strLen);
        memcpy(&dump.pBuffer->str[at], sfx_name.pBuffer->str, sfx_name.pBuffer->strLen);
        dsTSTRING_Insert(&dump, dump.pBuffer->strLen, "' obj list: ", -1);
        if ( names && npoints > 0 )
        {
            const char **cursor = names;
            int remaining = npoints;
            do
            {
                if ( *cursor )
                {
                    dsTSTRING_Insert(&dump, dump.pBuffer->strLen, *cursor, -1);
                    dsTSTRING_Insert(&dump, dump.pBuffer->strLen, ", ", -1);
                }
                --remaining;
                ++cursor;
            }
            while ( remaining );
        }
        cmp = dsCMP();
        dsVECTOR_TSTR_InsertSorted(&hcexUsedSfx, &dump, &cmp, INS_DUP_IGNORE);
        tstring_release(&dump);
    }

    if ( !sfx_name.pBuffer->strLen )
    {
        tstring_release(&sfx_name);
        goto cleanup;
    }

    /* "sfx_slip_sand" underwater becomes the vehicle-water variant */
    if ( tstring_equals(sfx_name.pBuffer, "sfx_slip_sand") == 0 )
    {
        if ( hcex_is_under_water(positions->x, positions->y, positions->z) )
        {
            dsTSTRING_flat tmp;
            tmp.pBuffer = 0;
            dsTSTRING_UnsafeInit(&tmp, "sfx_slip_vehicle_water", -1, 0);
            dsTSTRING_assign(&sfx_name, &tmp);
            tstring_release(&tmp);
        }
    }

    if ( plr_idx != -1 )
    {
        /* First-person view: set the sml state on the matching cyborg_fp model. When this player's view is
         * the locally-controlled one, the model's "shown to view 1" slot (0x80) gates it; otherwise the
         * "shown to view 2" slot (0x100) does. */
        if ( hcex_coop_local_player_index < 0 ? (plr_idx == 0 || plr_idx == 1)
                                              : (hcex_coop_local_player_index == plr_idx) )
        {
            int state_mask = (hcex_coop_local_player_index < 0 ? (plr_idx == 0)
                                                               : (hcex_coop_local_player_index == plr_idx))
                             ? 0x80 : 0x100;
            int i;
            for ( i = 0; i < fpModels.nElem; ++i )
            {
                animINST *inst = dsVECTOR_FPM_index(&fpModels, i)->pInst;
                entENTITY *ent = inst->pEnt;
                /* Set on the first eligible model whose template name is NOT "cyborg_fp" (matches the
                 * decompiler: it breaks/sets on the name mismatch). */
                if ( ent && (inst->state2 & state_mask) == 0
                        && cstr_cmp(inst->pTpl->name, "cyborg_fp") != 0 )
                {
                    sml_STATE_Set((sml_STATE *)&ent->stateSml, &sfx_name); /* typed: iaIACTOR::stateSml @0x24 */
                    break;
                }
            }
        }
        tstring_release(&sfx_name);
        goto cleanup;
    }

    if ( obj_follow != -1 )
    {
        /* Grunt Birthday Party skull: headshot-killed grunt plays konfetti + laugh. */
        if ( hcex_skull_GruntBirthdayParty && killed_body_part == _damage_part_head )
        {
            animINST *inst;
            sprintf_0(inst_name, "id%x", obj_follow);
            inst = scnSCENE_FindInstName(gsScenePtr, inst_name);
            if ( inst && cmp_name_equals(inst->pTpl->name, "grunt") )
            {
                snd_BUFFER_PARAMS params;
                dsSTRID strid;
                snd_BUFFER *buffer;
                dsTSTRING_flat konfetti;
                konfetti.pBuffer = 0;
                dsTSTRING_UnsafeInit(&konfetti, "sfx_konfetti", -1, 0);
                dsTSTRING_assign(&sfx_name, &konfetti);
                tstring_release(&konfetti);

                snd_BUFFER_params_ctor(&params);
                dsSTRID_init(&strid, "skull_laugh", 0);
                buffer = snd_SYSTEM_GetBuffer(snd_System, strid.id, &params);
                if ( buffer )
                {
                    snd_BUFFER_Play(buffer);
                    snd_BUFFER_Release(buffer);
                }
                tstring_release(&params.dbgInfo);
            }
        }

        if ( hcex_is_weapon(obj_follow) )
        {
            HCEX_OBJ *obj = hcex_obj_get(obj_follow);
            if ( !obj )
                goto release_sfx;
            if ( obj->pInst && obj->pInst->pEnt && entENTITY_GetNumSfx(obj->pInst->pEnt) > 0 )
            {
                sml_STATE_Set((sml_STATE *)&obj->pInst->pEnt->stateSml, &sfx_name); /* typed: iaIACTOR::stateSml @0x24 */
                goto release_sfx;
            }
        }

        if ( hcex_is_vehicle(obj_follow)
                && tstring_equals(sfx_name.pBuffer, "sfx_vhc_wa_warthog_gun_fire") == 0 )
        {
            HCEX_OBJ *obj = hcex_obj_get(obj_follow);
            if ( !obj )
                goto release_sfx;
            if ( obj->pInst && obj->pInst->pEnt && entENTITY_GetNumSfx(obj->pInst->pEnt) > 0 )
            {
                sml_STATE_Set((sml_STATE *)&obj->pInst->pEnt->stateSml, &sfx_name); /* typed: iaIACTOR::stateSml @0x24 */
                goto release_sfx;
            }
        }
    }

    if ( !dscDESC_FAMILY_FindBrand(&iaFamily, &sfx_name) )
    {
        cmp = dsCMP();
        dsVECTOR_TSTR_InsertSorted(&hcexMissedSfx, &sfx_name, &cmp, INS_DUP_IGNORE);
        tstring_release(&sfx_name);
        goto cleanup;
    }

    if ( obj_follow == -1 )
    {
        HCEX_EFFECT *effect;
        int i;
        {
            /* DEVIATION: the ctor/dtor pair around the push was spelled as explicit
             * HCEX_EFFECT_ctor/_dtor calls on a POD twin of HCEX_EFFECT. HCEX_EFFECT now carries
             * its real members (src/headers/hcex/HCEX_EFFECT.h), so the compiler emits
             * ??0HCEX_EFFECT@@ @0x823D6AA4 at scope entry and ??1HCEX_EFFECT@@ @0x823D6B54 at
             * scope exit — the same two calls, at the same two points, without duplicating them. */
            HCEX_EFFECT scratch;
            effect = dsVECTOR_EFF_PushBack(&hcexCreateEffects, &scratch);
        }
        dsTSTRING_assign(&effect->hcexName, &sfx_name);
        dsVECTOR_LOC_Resize(&effect->locations, npoints);
        for ( i = 0; i < npoints; ++i )
        {
            HCEX_EFFECT_LOC *loc = dsVECTOR_LOC_index(&effect->locations, i);
            const char *loc_name;
            dsTSTRING_flat tmp;
            loc->pos = positions[i];   /* see header note re: (positions - vectors) offset form */
            dsVECTOR_LOC_index(&effect->locations, i)->vec = vectors[i];
            loc_name = (names && names[i]) ? names[i] : empty_string;
            tmp.pBuffer = 0;
            dsTSTRING_UnsafeInit(&tmp, loc_name, -1, 0);
            dsTSTRING_assign(&dsVECTOR_LOC_index(&effect->locations, i)->name, &tmp);
            tstring_release(&tmp);
        }
    }
    else
    {
        dsTSTRING_flat held;
        entENTITY *ent;
        ++sfx_name.pBuffer->refCount;
        held.pBuffer = sfx_name.pBuffer;
        ent = hcex_create_effect_impl(&held, obj_follow, positions);
        if ( ent )
        {
            hcex_prepare_locations(ent->pInst, positions, vectors, names, npoints);
            hcex_set_follow(ent, obj_follow);
        }
    }

release_sfx:
    tstring_release(&sfx_name);

cleanup:
    if ( name_buffer->refCount-- == 1 )
        dlFree(name_buffer);
}
