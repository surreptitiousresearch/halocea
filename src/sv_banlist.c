/* sv_banlist @0x83766858 — server console command thunk: print the current ban list to the console. */

/* DEVIATION: this row was attested `int` while being a pure 1-instruction tail branch
 * (b banlist_print_to_console@0x83766858) — under the tail-thunk rule its return type IS the
 * callee's, and banlist_print_to_console is void (r3 at its exit@0x83766118 is ship_terminal_printf's
 * residue, bl@0x83766104). No consumer disagrees: the single call site sv_banlist_evaluate@0x8372E69C
 * discards r3 two instructions later (li r4,0@0x8372E6A0 + mr r3,r31@0x8372E6A4 feed
 * bl hs_return@0x8372E6A8 a literal 0), and the script table entry sv_banlist_definition@0x8211C7CC
 * declares return_type = hs_type_void. The local extern below had also said `int`, contradicting the
 * definition in src/banlist_print_to_console.c. */
extern void banlist_print_to_console(void);

void sv_banlist(void)
{
    banlist_print_to_console();
}
