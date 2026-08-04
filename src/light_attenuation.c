/* DEVIATION: decompiler double-return idiom; replaced with direct float arithmetic */
float light_attenuation(float radius, float distance)
{
    return 1.0f - (distance * distance) / (radius * radius);
}
