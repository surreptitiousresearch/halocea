/* arcsine @0x83705540 */
#include <math.h>

/* DEVIATION: decompiler shows double param; prototype is float; PPC FPRs hold both */
float arcsine(float x)
{
    return (float)asin((double)x);
}
