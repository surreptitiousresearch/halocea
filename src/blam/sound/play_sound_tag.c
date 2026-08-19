/* play_sound_tag @0x83732538 */
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void play_sound_tag(int definition_index)
{
    if (definition_index != -1)
        unspatialized_impulse_sound_new(definition_index, 1.0);
}
