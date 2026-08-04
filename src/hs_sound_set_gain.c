/* hs_sound_set_gain @0x837F88B0 — set the playback gain for a named sound tag, if it currently has a gain
 * reference. */

extern float * hs_sound_get_gain_reference(const char *tag_name);

void hs_sound_set_gain(const char *tag_name, float gain)
{
    float *gain_reference = hs_sound_get_gain_reference(tag_name);
    if ( gain_reference )
        *gain_reference = gain;
}
