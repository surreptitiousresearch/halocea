/* main_loop_init @0x8368B9C8 — one-time main-loop initialization: run the two init phases in order.
 *
 * Deviation: the decompiler chains init1/init2 through r3 (both are void no-arg); unchained into
 * independent calls. */

extern void main_loop_init1(void);
extern void main_loop_init2(void);

void main_loop_init(void)
{
    main_loop_init1();
    main_loop_init2();
}
