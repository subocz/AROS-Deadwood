/*
    (C) 1995-96 AROS - The Amiga Research OS
    $Id$

    Desc: Remove a memory handler.
    Lang: english
*/
#include <exec/execbase.h>
#include <aros/libcall.h>
#include <proto/exec.h>

/*****************************************************************************

    NAME */

	AROS_LH1(void, RemMemHandler,

/*  SYNOPSIS */
	AROS_LHA(struct Interrupt *, memHandler, A1),

/*  LOCATION */
	struct ExecBase *, SysBase, 130, Exec)

/*  FUNCTION
	Remove some function added with AddMemHandler again.

    INPUTS
	memHandler - The same Interrupt structure you gave to AddMemHandler().

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO

    INTERNALS

    HISTORY
       18-10-95    created by m. fleischer

******************************************************************************/
{
    AROS_LIBFUNC_INIT

    /* Protect the low memory handler list */
    Forbid();
	/* Nothing spectacular: Just remove node */
	Remove(&memHandler->is_Node);
    Permit();
    AROS_LIBFUNC_EXIT
} /* RemMemHandler */

