int character_in_list(char c, __int16 list_count, const char *list)
{
    for (__int16 i = 0; i < list_count; ++i)
    {
        if (c == list[i])
            return 1;
    }
    return 0;
}
