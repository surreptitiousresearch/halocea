/* signed_angular_difference @ 0x8369DBF0 — shortest signed angle from angle1 to
 * angle2, wrapped into (-pi, pi]. (f1=angle1, f2=angle2.) */

#define K_PI  3.1415927f
#define K_2PI 6.2831855f

float signed_angular_difference(float angle1, float angle2)
{
    float difference = angle2 - angle1;
    if (difference >= K_PI)
        difference -= K_2PI;
    if (difference <= -K_PI)
        difference += K_2PI;
    return difference;
}
