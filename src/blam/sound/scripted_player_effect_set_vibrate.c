/* scripted_player_effect_set_vibrate @0x83737808 — thunk: set the scripted controller-vibration motor
 * intensities (left, right). */

extern void vibrate_player_set_scripted_values(float left, float right);

void scripted_player_effect_set_vibrate(float left_frequency, float right_frequency)
{
    vibrate_player_set_scripted_values(left_frequency, right_frequency);
}
