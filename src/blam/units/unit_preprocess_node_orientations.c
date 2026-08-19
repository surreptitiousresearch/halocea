/* unit_preprocess_node_orientations @0x836CD898 — apply a unit's per-frame animation-graph overlays onto
 * `node_orientations`, after the base pose has already been built. Always applies the unit's current
 * action/overlay-action/soft-ping animations (unit_animation.action_animation/overlay_action_animation/
 * soft_ping_animation). Then, if the unit definition doesn't disable it (unit tag flags bit 0x800) and the
 * unit has a seat selected (unit_animation.seat_index != -1): applies an emotion-pose overlay (an explicit
 * frame chosen by emotion_index, on either the script-overridden animation or the seat's fixed slot-11
 * animation), a mouth/talk overlay scaled by the live mouth_aperture value (seat's fixed slot 10), and three
 * additive overlays (seat slots 2-4) each scaled by the corresponding axis of seat_acceleration — a per-axis
 * recoil/kick effect layered onto the base pose. Finally, unless the unit tag flags bit 0x400 disables it and
 * the unit's animation state/action allow it: builds the unit's world orientation frame, transforms the
 * aiming_vector into it to get a yaw/pitch aiming angle (fed to aiming_screen_apply along with the current
 * weapon class's aiming bounds), then does the same for looking_vector, whose yaw/pitch is expressed
 * *relative to* the aiming angle just computed (fed to aiming_screen_apply with the seat's own looking
 * bounds). Both screens write their resolved yaw/pitch bound rectangles back into unit_animation for
 * unit_clip_to_aiming_bounds to consume, and set their "with_euler_screen" applied flag.
 *
 * Unlike vehicle_preprocess_node_orientations/device_preprocess_node_orientations, this function does not
 * check the animation graph tag reference for validity before use — a unit's object definition is assumed to
 * always carry one (disasm-confirmed: no such check exists at 0x836CD8F4-0x836CD908).
 *
 * DEVIATIONS (disasm-resolved, 0x836CD8D4-0x836CDEAC):
 *  - node_orientations (r4) is saved to r25 at entry and passed unchanged to every apply call. The
 *    decompiler lost this: it renders two DIFFERENT things as pseudo-variables "v6"/"v7" that happen to
 *    share r4/r5 with node_orientations at various points (v6 is really the dead definition_index value
 *    used only to seed the TAG_INSTANCE lookup; v7 is really unit_animation.action_animation.index) and then
 *    wrongly reuses those names as the node_orientations argument in two calls. Every apply call's
 *    node_orientations argument is r25, confirmed by the disassembly's own "# node_orientations" annotations.
 *  - Every `__int64`/HIDWORD/LODWORD-punned local (v29/v30/v32/v35/v39/v45/v46 etc.) is the standard PPC
 *    int16-to-float conversion idiom (sign-extend, store/reload through a 64-bit stack slot, fcfid, frsp) —
 *    plain `(float)(__int16)field` casts once unwound; none of the 64-bit views carry real data.
 *  - The decompiler's `v26[4].<field>` accesses (v26 being a mis-scaled `animation_aiming_screen_bounds *`)
 *    are correct once its actual DB size (24 bytes, confirmed via `types` — not the outer 188-byte per-weapon
 *    record stride) is accounted for: `v26 + 4` (4 * 24 = 96 bytes) lands exactly on the weapon class's own
 *    `aiming_screen_bounds` sub-struct (`animation_graph_weapon_class` offset 0x60), matching the disasm's
 *    direct `r30 + 0x60` register alias (renamed r29). Reproduced here as `&weapon_class->aiming_screen_bounds`.
 *  - The seat's `animations` tag_block is an array of `animation_graph_animation_index` (DB type, 2 bytes,
 *    one __int16), not a bare int16_t array; its slots are the `unit_seat_animation` enum (11 = emotions,
 *    10 = mouth_aperture, 2..4 = acceleration front_back/left_right/up_down). Disasm 0x836CD9C8/0x836CDA4C
 *    load .address (+0x44) then lhz at +0x16 / +0x14, and 0x836CDAB0 scales the loop slot by 2 (slwi 1).
 *  - `unit_datum.h`'s `_unit_datum` was previously only a partial/padded stub; this session fleshed it out to
 *    the full database layout (all field names/offsets DB-verified via `types_members`), which is what makes
 *    every field below a named access rather than a raw offset. New nested types `unit_animation`,
 *    `animation_graph_unit_seat`, `animation_graph_weapon_class`, and a filled-in `animation_aiming_screen_bounds`
 *    (previously an opaque forward declaration) were pulled the same way. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/animation_graph_animation_index.h"
#include "headers/animation_aiming_screen_bounds.h"
#include "headers/animation.h"
#include "headers/unit_seat_animation.h"
#include "headers/real_orientation.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_definition_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"

extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern real_vector3d *matrix4x3_inverse_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void replacement_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply_scaled(const animation *animation, int16_t frame_index, float animation_scale, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous(const animation *animation, float real_frame_index, real_orientation *node_orientations);
extern void aiming_screen_apply(const animation *animation, const animation_aiming_screen_bounds *aiming_screen, float direction, float elevation, real_orientation *node_orientations);

void unit_preprocess_node_orientations(int object_index, real_orientation *node_orientations)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    unit_definition *unit_def = TAG_GET(unit_definition, unit->definition_index);
    /* animation_graph tag_reference lives in the shared _object_definition preamble (object.animation_graph,
     * byte 68), same as vehicle/device_preprocess_node_orientations. */
    /* recovered: *(int *)((char *)unit_def + 68) -> unit_def->object.animation_graph.index */
    animation_graph *graph = TAG_GET(animation_graph, unit_def->object.animation_graph.index);

    /* Always-run: the unit's current action / overlay-action / soft-ping animations. */
    if (unit->unit.animation.action_animation.index != -1)
        replacement_animation_apply(
                &((const animation *)graph->animations.address)[unit->unit.animation.action_animation.index],
                unit->unit.animation.action_animation.frame_index, node_orientations);

    if (unit->unit.animation.overlay_action_animation.index != -1)
        overlay_animation_apply(
                &((const animation *)graph->animations.address)[unit->unit.animation.overlay_action_animation.index],
                unit->unit.animation.overlay_action_animation.frame_index, node_orientations);

    if (unit->unit.animation.soft_ping_animation.index != -1)
        overlay_animation_apply(
                &((const animation *)graph->animations.address)[unit->unit.animation.soft_ping_animation.index],
                unit->unit.animation.soft_ping_animation.frame_index, node_orientations);

    unit->unit.animation.aiming_with_euler_screen = 0;
    unit->unit.animation.looking_with_euler_screen = 0;

    if ((unit_def->unit.flags & (1u << _unit_definition_simple_creature_bit)) != 0)
        return;
    if (unit->unit.animation.seat_index == -1)
        return;

    animation_graph_unit_seat *seat =
        &((animation_graph_unit_seat *)graph->unit_seats.address)[unit->unit.animation.seat_index];
    char emotion_index = unit->unit.animation.emotion_index;

    /* Emotion pose overlay: script override (override_emotion_animation_index), else the seat's fixed
     * slot 11; emotion_index doubles as the explicit frame to display. */
    if (emotion_index != -1)
    {
        int16_t emotion_animation_index = seat->animations.count <= _unit_seat_animation_emotions
                ? -1
                : ((const animation_graph_animation_index *)seat->animations.address)
                        [_unit_seat_animation_emotions].animation_index;
        if (unit->unit.override_emotion_animation_index != -1)
            emotion_animation_index = unit->unit.override_emotion_animation_index;

        if (emotion_animation_index != -1)
        {
            const animation *emotion_animation =
                    &((const animation *)graph->animations.address)[emotion_animation_index];
            if (emotion_index >= 0 && emotion_index < emotion_animation->frame_count)
                overlay_animation_apply(emotion_animation, emotion_index, node_orientations);
        }
    }

    /* Mouth/talk overlay: seat's fixed slot 10, scaled by the live mouth_aperture value. */
    if (unit->unit.mouth_aperture > 0.0f)
    {
        int16_t mouth_animation_index = seat->animations.count <= _unit_seat_animation_mouth_aperture
                ? -1
                : ((const animation_graph_animation_index *)seat->animations.address)
                        [_unit_seat_animation_mouth_aperture].animation_index;
        if (mouth_animation_index != -1)
            overlay_animation_apply_scaled(
                    &((const animation *)graph->animations.address)[mouth_animation_index],
                    0, unit->unit.mouth_aperture, node_orientations);
    }

    /* Three additive overlays (seat slots 2-4), each scaled by the corresponding axis of seat_acceleration —
     * a per-axis recoil/kick effect layered onto the base pose. */
    if ((unit->unit.animation.flags & (1u << _unit_animation_showing_acceleration_bit)) != 0)
    {
        for (int i = 0; i < 3; ++i)
        {
            int slot = _unit_seat_animation_acceleration_front_back + i;
            int16_t additive_animation_index = (slot < 0 || slot >= seat->animations.count)
                    ? -1
                    : ((const animation_graph_animation_index *)seat->animations.address)
                            [slot].animation_index;
            if (additive_animation_index != -1)
            {
                const animation *additive_animation =
                        &((const animation *)graph->animations.address)[additive_animation_index];
                float weight = unit->unit.seat_acceleration.n[i];
                overlay_animation_apply_continuous(additive_animation,
                        (float)(additive_animation->frame_count - 1) * weight, node_orientations);
            }
        }
    }

    if ((unit_def->unit.flags & (1u << _unit_definition_has_no_aiming_bit)) != 0)
        return;

    unsigned char state = (unsigned char)unit->unit.animation.state;
    unsigned char aiming_and_looking_enabled = unit->unit.animation.action == 0;
    /* disable aiming/looking during hard-ping/death/committed-action states (0x17.._unit_state_resurrect_back)
     * and the leap-melee state */
    if (state >= _unit_state_hard_ping && (state <= _unit_state_resurrect_back || state == _unit_state_leap_melee))
        aiming_and_looking_enabled = 0;
    if (!aiming_and_looking_enabled)
        return;

    real_euler_angles2d aiming_angles = *global_zero_angles2d;

    if (debug_off_aiming_object_index != object_index && unit->unit.animation.aiming_screen_index != -1)
    {
        real_matrix4x3 frame;
        frame.scale = 1.0f;
        object_get_orientation(object_index, &frame.forward, &frame.up);
        /* left = forward x up */
        frame.left.n[0] = frame.forward.n[2] * frame.up.n[1] - frame.up.n[2] * frame.forward.n[1];
        frame.left.n[1] = frame.forward.n[0] * frame.up.n[2] - frame.forward.n[2] * frame.up.n[0];
        frame.left.n[2] = frame.up.n[0] * frame.forward.n[1] - frame.forward.n[0] * frame.up.n[1];
        frame.position = *global_origin3d;

        real_vector3d local_aiming_vector;
        matrix4x3_inverse_transform_normal(&frame, &unit->unit.aiming_vector, &local_aiming_vector);
        euler_angles2d_from_vector3d(&aiming_angles, &local_aiming_vector);
        unit->unit.animation.aiming_with_euler_screen = 1;

        animation_graph_weapon_class *weapon_class =
            &((animation_graph_weapon_class *)seat->weapon_classes.address)[unit->unit.animation.weapon_index];

        unit->unit.animation.aiming_screen_bounds.x0 = -((float)weapon_class->aiming_screen_bounds.negative_yaw_frame_count
                * weapon_class->aiming_screen_bounds.negative_yaw_delta);
        unit->unit.animation.aiming_screen_bounds.x1 = (float)weapon_class->aiming_screen_bounds.positive_yaw_frame_count
                * weapon_class->aiming_screen_bounds.positive_yaw_delta;
        unit->unit.animation.aiming_screen_bounds.y0 = -((float)weapon_class->aiming_screen_bounds.negative_pitch_frame_count
                * weapon_class->aiming_screen_bounds.negative_pitch_delta);
        unit->unit.animation.aiming_screen_bounds.y1 = (float)weapon_class->aiming_screen_bounds.positive_pitch_frame_count
                * weapon_class->aiming_screen_bounds.positive_pitch_delta;

        aiming_screen_apply(
                &((const animation *)graph->animations.address)[unit->unit.animation.aiming_screen_index],
                &weapon_class->aiming_screen_bounds, aiming_angles.yaw, aiming_angles.pitch, node_orientations);
    }

    if (debug_off_looking_object_index != object_index
            && (unit->unit.current_weapon_index != -1 || unit->unit.player_index != -1)
            && unit->unit.animation.looking_screen_index != -1)
    {
        real_matrix4x3 frame;
        frame.scale = 1.0f;
        object_get_orientation(object_index, &frame.forward, &frame.up);
        frame.left.n[0] = frame.forward.n[2] * frame.up.n[1] - frame.up.n[2] * frame.forward.n[1];
        frame.left.n[1] = frame.forward.n[0] * frame.up.n[2] - frame.forward.n[2] * frame.up.n[0];
        frame.left.n[2] = frame.up.n[0] * frame.forward.n[1] - frame.forward.n[0] * frame.up.n[1];
        frame.position = *global_origin3d;

        real_vector3d local_looking_vector;
        matrix4x3_inverse_transform_normal(&frame, &unit->unit.looking_vector, &local_looking_vector);
        real_euler_angles2d looking_angles;
        euler_angles2d_from_vector3d(&looking_angles, &local_looking_vector);
        unit->unit.animation.looking_with_euler_screen = 1;

        /* direction/elevation are the looking angle RELATIVE to the aiming angle computed above (which is
         * the zero-angle default if the aiming block above was skipped). */
        float direction = looking_angles.yaw - aiming_angles.yaw;
        float elevation = looking_angles.pitch - aiming_angles.pitch;

        unit->unit.animation.looking_screen_bounds.x0 = -((float)seat->looking_screen_bounds.negative_yaw_frame_count
                * seat->looking_screen_bounds.negative_yaw_delta);
        unit->unit.animation.looking_screen_bounds.x1 = (float)seat->looking_screen_bounds.positive_yaw_frame_count
                * seat->looking_screen_bounds.positive_yaw_delta;
        unit->unit.animation.looking_screen_bounds.y0 = -((float)seat->looking_screen_bounds.negative_pitch_frame_count
                * seat->looking_screen_bounds.negative_pitch_delta);
        unit->unit.animation.looking_screen_bounds.y1 = (float)seat->looking_screen_bounds.positive_pitch_frame_count
                * seat->looking_screen_bounds.positive_pitch_delta;

        aiming_screen_apply(
                &((const animation *)graph->animations.address)[unit->unit.animation.looking_screen_index],
                &seat->looking_screen_bounds, direction, elevation, node_orientations);
    }
}
