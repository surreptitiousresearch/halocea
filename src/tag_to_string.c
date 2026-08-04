char * tag_to_string(unsigned int t, char *s)
{
    *(unsigned int *)s = t;
    s[4] = 0;
    return s;
}
