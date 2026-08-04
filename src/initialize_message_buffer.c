/* initialize_message_buffer @0x83810578 — set up a 2-byte-prefixed message buffer: report the maximum
 * content size in bits (8 * (buffer_size - 2), reserving the 2-byte length prefix) and return a pointer past
 * the prefix where content is written. */

char *initialize_message_buffer(char *buffer, int buffer_size, int *maximum_content_size)
{
    *maximum_content_size = 8 * (buffer_size - 2);
    return buffer + 2;
}
