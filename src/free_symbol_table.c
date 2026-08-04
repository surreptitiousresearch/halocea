#include "headers/debug_symbol_table.h"

extern void dlFree(void *ptr);

void free_symbol_table(debug_symbol_table *symbol_table)
{
    if (symbol_table->string_storage)
        dlFree(symbol_table->string_storage);
    if (symbol_table->symbols)
        dlFree(symbol_table->symbols);
    symbol_table->number_of_symbols = 0;
    symbol_table->string_storage = 0;
    symbol_table->symbols = 0;
}
