/* minValueRoundedUpTo1 @ 0x836049A8 — Blam engine.
 * Steps a float up by 0.01 until its integer truncation is non-zero (i.e. it
 * reaches 1.0), clamping to 1.0. Effectively returns 1.0. */

float minValueRoundedUpTo1(void)
{
    float value = 0.0f;

    /* loop while the integer truncation of value is still zero */
    while ((int)value == 0)
    {
        value += 0.0099999998f; /* 0x3c23d70a */
        if (value >= 1.1f)      /* 0x3f8ccccd */
        {
            value = 1.0f;
            break;
        }
    }
    return value;
}
