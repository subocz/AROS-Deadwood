/*
    Copyright (C) 1995-97 AROS - The Amiga Research OS
    $Id$

    Desc: GetSysTime() - Find out what time it is.
    Lang: english
*/
#include "timer_intern.h"
#include <proto/exec.h>

/*****************************************************************************

    NAME */
#include <devices/timer.h>
#include <proto/timer.h>

	AROS_LH1(void, GetSysTime,

/*  SYNOPSIS */
	AROS_LHA(struct timeval *, dest, A0),

/*  LOCATION */
	struct TimerBase *, TimerBase, 11, Timer)

/*  FUNCTION
	GetSysTime() will fill in the supplied timeval with the current
	system time.

    INPUTS
	dest    -   A pointer to the timeval you want the time stored in.

    RESULT
	The timeval "dest" will be filled with the current system time.

    NOTES
	This function is safe to call from interrupts.

    EXAMPLE

    BUGS

    SEE ALSO
	TR_GETSYSTIME, TR_SETSYSTIME

    INTERNALS

    HISTORY
	18-02-1997  iaint   Implemented.

*****************************************************************************/
{
    AROS_LIBFUNC_INIT
    AROS_LIBBASE_EXT_DECL(struct Library *,TimerBase)

    Disable();
    dest->tv_secs = TimerBase->tb_CurrentTime.tv_secs;
    dest->tv_micro = TimerBase->tb_CurrentTime.tv_micro;
    TimerBase->tb_CurrentTime.tv_micro += 1;
    if(TimerBase->tb_CurrentTime.tv_micro > 999999)
    {
	TimerBase->tb_CurrentTime.tv_secs += 1;
	/* MUST be zero since we are only adding 1 */
	TimerBase->tb_CurrentTime.tv_micro = 0;
    }
    Enable();

    AROS_LIBFUNC_EXIT
} /* GetSysTime */
