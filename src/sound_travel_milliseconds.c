/* sound_travel_milliseconds — propagation delay for a sound to travel `distance` world units.
 * 8.9647064 is milliseconds per world unit at the speed of sound (world unit = 10 ft ~= 3.048 m,
 * v_sound ~= 340 m/s -> ~8.96 ms/unit); i.e. the reciprocal of the speed of sound in units/ms. */
int sound_travel_milliseconds(float distance)
{
    return (int)(distance * 8.9647064f);   /* ms per world unit (speed-of-sound reciprocal) */
}
