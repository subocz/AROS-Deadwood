/*
    Copyright � 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Close a device.
    Lang: english
*/
#include <aros/config.h>
#include <exec/execbase.h>
#include <exec/io.h>
#include <exec/devices.h>
#include <dos/dos.h>
#include <aros/libcall.h>
#include <proto/exec.h>

#include "exec_debug.h"
#ifndef DEBUG_CloseDevice
#   define DEBUG_CloseDevice 0
#endif
#undef DEBUG
#if DEBUG_CloseDevice
#   define DEBUG 1
#endif
#include <aros/debug.h>
#undef kprintf

#if (AROS_FLAVOUR & AROS_FLAVOUR_NATIVE)
#   define NATIVE(x)        x
#else
#   define NATIVE(x)        /* eps */
#endif

/*****************************************************************************

    NAME */

	AROS_LH1(void, CloseDevice,

/*  SYNOPSIS */
	AROS_LHA(struct IORequest *, iORequest, A1),

/*  LOCATION */
	struct ExecBase *, SysBase, 75, Exec)

/*  FUNCTION
	Closes a previously opened device. Any outstanding I/O requests must
	be finished. It is safe to call CloseDevice with a cleared iorequest
	structure or one that failed to open.

    INPUTS
	iORequest - Pointer to iorequest structure.

    RESULT

    NOTES

    EXAMPLE

    BUGS

    SEE ALSO
	OpenDevice().

    INTERNALS

    HISTORY

******************************************************************************/
{
    AROS_LIBFUNC_INIT
    NATIVE(BPTR dc_ret;)

    D(bug("CloseDevice $%lx $%lx (\"%s\") by \"%s\"\n", iORequest, iORequest->io_Device,
	iORequest->io_Device ? iORequest->io_Device->dd_Library.lib_Node.ln_Name : "(null)",
	SysBase->ThisTask->tc_Node.ln_Name));

    /* Single-thread the close routine. */
    Forbid();

    /* Something to do? */
    if(iORequest->io_Device!=NULL)
    {
	NATIVE(dc_ret =) AROS_LVO_CALL1(BPTR,
	    AROS_LCA(struct IORequest *,iORequest, A1),
	    struct Device *,iORequest->io_Device,2,
	);
	/*
	    Normally you'd expect the device to be expunged if this returns
	    non-zero, but this is only exec which doesn't know anything about
	    seglists - therefore dos.library has to SetFunction() into this
	    vector for the additional functionality.
	*/

	/* Trash device field */
	iORequest->io_Device=(struct Device *)-1;
    }
#if (AROS_FLAVOUR & AROS_FLAVOUR_NATIVE)
    else
    {
	/* local vars not guaranteed to be initialised to 0 */
	dc_ret = 0;
    }
#endif

    /* All done. */
    Permit();

#if (AROS_FLAVOUR & AROS_FLAVOUR_NATIVE)
    /*
	Kludge to force the seglist to register d0. Ramlib patches this
	vector for seglist expunge capability and expects the seglist in
	d0 after it has called the original (this) function.
	Also see CloseLibrary().
    */
    {
	/* Put the library base in register d0 */
	register BPTR ret __asm("d0") = dc_ret;

	/* Make sure the above assignment isn't optimized away */
	asm volatile("": : "r" (ret));
    }
#endif

    AROS_LIBFUNC_EXIT
} /* CloseDevice */

