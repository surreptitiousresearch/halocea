/* console_is_token_supar_secret @0x837224F0 */
#include <stdint.h>
uint8_t console_is_token_supar_secret(const char *token)
{
    const char *secret = "profile_load";
    while ( *token && *token == *secret )
    {
        ++token;
        ++secret;
    }
    return *token == *secret;
}
