extern int bits_needed[];

int bits_needed_for_value(int value)
{
    return bits_needed[value];
}
