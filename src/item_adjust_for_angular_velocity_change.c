/* item_adjust_for_angular_velocity_change @0x83757EDC — recomputes an item's cached spin axis and
 * the sin/cos of its angular speed from its current angular velocity. When the angular velocity is
 * zero the spin is cleared (item _has_nonzero_angular_velocity flag off, sin 0, cos 1); otherwise the
 * axis is normalized (unless the object is at rest, in which case the cached axis is left frozen) and
 * sin/cos of the speed are cached for the renderer.
 *
 * DEVIATION: the decompiler invented a long double scratch parameter (written before read); the DB
 * prototype is single-argument. Raw char*+offset access rewritten to typed item_datum fields
 * (DB-verified: object.flags @item+16 bit 0x20 = _object_at_rest_bit; item.flags bit 0x04 =
 * _item_has_nonzero_angular_velocity_bit). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/item_datum.h"
#include "headers/item_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern double sin(double x);
extern double cos(double x);
extern float __fsqrts(float x);

void item_adjust_for_angular_velocity_change(int object_index)
{
    item_datum *item = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    float wx = item->object.angular_velocity.n[0];
    float wy = item->object.angular_velocity.n[1];
    float wz = item->object.angular_velocity.n[2];
    float speed = __fsqrts(wz * wz + (wx * wx + wy * wy));

    if (speed == 0.0f)
    {
        item->item.rotation_sine = 0.0f;
        item->item.flags &= ~(1u << _item_has_nonzero_angular_velocity_bit);
        item->item.rotation_cosine = 1.0f;
    }
    else
    {
        int at_rest = item->object.flags & (1u << _object_at_rest_bit);
        item->item.flags |= (1u << _item_has_nonzero_angular_velocity_bit);
        if (!at_rest)
        {
            float inv = 1.0f / speed;
            item->item.rotation_axis.n[0] = wx * inv;
            item->item.rotation_axis.n[1] = wy * inv;
            item->item.rotation_axis.n[2] = wz * inv;
        }
        item->item.rotation_sine = (float)sin(speed);
        item->item.rotation_cosine = (float)cos(speed);
    }
}
