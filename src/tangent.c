#include <math.h>

/* DEVIATION: decompiler shows double param; prototype is float; PPC FPRs hold both */
float tangent(float angle)
{
    return (float)tan((double)angle);
}
