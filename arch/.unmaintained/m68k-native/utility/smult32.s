/*
    Copyright (C) 1995-97 AROS - The Amiga Research OS
    $Id$

    Desc: Utility 32 bit multiplication routines. m68k version.
    Lang: english
*/

#include "machine.i"

/* SMult32():
    These are the signed/unsigned 32 bit multiplication routines.
    I have two different possibilities here since I could be running
    on a system without the corresponding muls.l/mulu.l calls.

    After some soul searching I am happy that these routines don't
    have to be separated between signed/unsigned since the sign is
    correct, or the number overflows.

    The native versions do make the difference however.

    What I do is SetFunction() the correct function in later.
*/

    .balign 4
    .globl  AROS_SLIB_ENTRY(SMult32_020,Utility)
    .type   AROS_SLIB_ENTRY(SMult32_020,Utility),@function
AROS_SLIB_ENTRY(SMult32_020,Utility):
    muls.l  d1,d0
    rts

/*
    This only contains the 020 version because the 000 versions of
    both S and U are the same, but I need the separate files so that
    the makefile tricks don't cause problems with duplicate symbols.
*/
