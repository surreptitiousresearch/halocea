/* update_ticks @0x836FB5E0 — advance a saturating 8-bit tick counter. When `down` is set the
 * counter is incremented, clamped to 255; otherwise it is reset to 0. */

typedef unsigned char byte;

void update_ticks(byte *ticks, byte down)
{
    int value;

    if ( down )
    {
        value = *ticks + 1;
        if ( value > 255 )
        {
            *ticks = 255;
            return;
        }
    }
    else
    {
        value = 0;
    }
    *ticks = (byte)value;
}
