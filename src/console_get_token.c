/* console_get_token @0x83722150 */
extern char *strrchr(const char *s, int c);

char * console_get_token(char *buffer)
{
    char *after_space = strrchr(buffer, ' ') + 1;
    char *after_paren = strrchr(buffer, '(') + 1;
    char *after_quote = strrchr(buffer, '"') + 1;
    char *token = buffer;

    if ( token <= after_space )
        token = after_space;
    if ( token <= after_paren )
        token = after_paren;
    if ( token > after_quote )
        return token;
    return after_quote;
}
