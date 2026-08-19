/* hs_sound_get_gain @0x837F8868 — return the current playback gain for a named sound tag, or 0 if it has no
 * gain reference. */

extern float * hs_sound_get_gain_reference(const char *tag_name);

float hs_sound_get_gain(const char *tag_name)
{
    float *gain_reference = hs_sound_get_gain_reference(tag_name);
    if ( gain_reference )
        return *gain_reference;
    return 0.0f;
}
