/* system_get_date_time_string @0x83762DA8 */
extern int _time32(int *timeptr);
extern const void *_localtime32(const int *ptime);
extern unsigned int strftime(char *string, unsigned int maxsize, const char *format, const void *timeptr);

unsigned int system_get_date_time_string(char *date_time_string, unsigned int max_length_in_bytes)
{
    int now;

    _time32(&now);
    return strftime(date_time_string, max_length_in_bytes, "%Y-%m-%d %H_%M_%S", _localtime32(&now));
}
