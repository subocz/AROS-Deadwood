/*
    (C) 1995-96 AROS - The Amiga Research OS
    $Id$

    Desc: Write a big endian string to a streamhook
    Lang: english
*/
#include <proto/dos.h>

/******************************************************************************

    NAME */
#include <stdio.h>
#include <aros/bigendianio.h>
#include <proto/alib.h>

	BOOL WriteString (

/*  SYNOPSIS */
	struct Hook * hook,
	STRPTR	      data,
	void	    * stream)

/*  FUNCTION
	Writes one big endian string to a streamhook.

    INPUTS
	hook - Write to this streamhook
	data - Data to be written
	stream - Stream passed to streamhook

    RESULT
	The function returns TRUE on success and FALSE otherwise.
	See IoErr() for the reason in case of an error.

    NOTES
	This function writes big endian values to a file even on little
	endian machines.

    EXAMPLE

    BUGS

    SEE ALSO
	ReadByte(), ReadWord(), ReadLong(), ReadFloat(), ReadDouble(),
	ReadString(), ReadStruct(), WriteByte(), WriteWord(), WriteLong(),
	WriteFloat(), WriteDouble(), WriteString(), WriteStruct()

    HISTORY

******************************************************************************/
{
    do
    {
	if (CallHook (hook, stream, BEIO_WRITE, *data) == EOF)
	    return FALSE;
    } while (*data ++);

    return TRUE;
} /* WriteString */

