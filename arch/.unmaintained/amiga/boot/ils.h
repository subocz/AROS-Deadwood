/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$

    Desc: Amiga bootloader -- InternalLoadSeg support routines
    Lang: C
*/

AROS_UFP4(LONG, ils_read,
    AROS_UFHA(BPTR,                handle,  D1),
    AROS_UFHA(void *,              buffer,  D2),
    AROS_UFHA(LONG,                length,  D3),
    AROS_UFHA(struct DosLibrary *, DOSBase, A6));

AROS_UFP3(void *, ils_alloc,
    AROS_UFHA(ULONG,             size,    D0),
    AROS_UFHA(ULONG,             attrib,  D1),
    AROS_UFHA(struct ExecBase *, SysBase, A6));

AROS_UFP3(void, ils_free,
    AROS_UFHA(void *,            block,   A1),
    AROS_UFHA(ULONG,             size,    D0),
    AROS_UFHA(struct ExecBase *, SysBase, A6));
