/* cosine @0x83699E10 */
#include <math.h>

/* DEVIATION: decompiler shows double param; prototype is float; PPC FPRs hold both */
float cosine(float angle)
{
    return (float)cos((double)angle);
}
