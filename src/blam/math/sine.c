/* sine @0x83699DE8 */
#include <math.h>

/* DEVIATION: decompiler shows double param; prototype is float; PPC FPRs hold both */
float sine(float angle)
{
    return (float)sin((double)angle);
}
