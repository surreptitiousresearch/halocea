extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

float *player_update_history_new(void)
{
    float *result = dlMalloc(0x24u, "D:\\Projects\\code\\HCEX\\sources\\networking\\player_update_history.c", 0x2Bu);

    for ( int i = 0; i < 9; i++ )
        result[i] = 0.0f;
    return result;
}
