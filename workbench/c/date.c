/*
    (C) 1997 AROS - The Amiga Replacement OS
    $Id$

    Desc: Date CLI command
    Lang: english
*/

#include <stdio.h>
#include <proto/exec.h>
#include <exec/errors.h>
#include <exec/io.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <dos/datetime.h>
#include <devices/timer.h>

static const char version[] = "$VER: Date 1.1 (5.3.1997)\n";

#define ARG_STRING "DAY,DATE,TIME,TO=VER/K"
#define ARG_DAY 0
#define ARG_DATE 1
#define ARG_TIME 2
#define ARG_VER 3
#define ARG_COUNT 4

int setdate(STRPTR day, STRPTR date, STRPTR time)
{
    int error = RETURN_OK;
    BYTE timererror;
    struct timerequest timerreq;

    timererror = OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)&timerreq, 0L);
    if (timererror == 0)
    {
        /* !!! */
        printf("Function not implemented, yet\n");
        error = RETURN_FAIL;
        CloseDevice((struct IORequest *)&timerreq);
    } else
    {
        printf("Date: Error opening timer.device\n");
        error = RETURN_FAIL;
    }

    return(error);
}

int printdate(STRPTR filename)
{
    BPTR file = Output();
    int ownfile = 0;
    int error = RETURN_OK;
    struct DateTime dt;
    char dowstring[LEN_DATSTRING], datestring[LEN_DATSTRING], timestring[LEN_DATSTRING], resstring[LEN_DATSTRING*3+1];

    if (filename != NULL)
    {
        file = Open(filename, MODE_NEWFILE);
        ownfile = 1;
    }
    if (file != NULL)
    {
        int pos = 0;

        DateStamp(&dt.dat_Stamp);
        dt.dat_Format = FORMAT_DEF;
        dt.dat_Flags = 0;
        dt.dat_StrDay = dowstring;
        dt.dat_StrDate = datestring;
        dt.dat_StrTime = timestring;
        DateToStr(&dt);
        CopyMem(dowstring, resstring, strlen(dowstring));
        pos += strlen(dowstring);
        resstring[pos++] = ' ';
        CopyMem(datestring, resstring + pos, strlen(datestring));
        pos += strlen(datestring);
        resstring[pos++] = ' ';
        resstring[pos++] = ' ';
        CopyMem(timestring, resstring + pos, strlen(timestring));
        pos += strlen(timestring);
        resstring[pos++] = 0x0a;
        if (Write(file, resstring, pos) < pos)
        {
            PrintFault(IoErr(), "Date");
            error = RETURN_FAIL;
        }
        if (ownfile == 1)
            Close(file);
    } else
    {
        PrintFault(IoErr(), "Date");
        error = RETURN_FAIL;
    }
    return(error);
}

int main(int argc, char **argv)
{
    int error = RETURN_OK;
    STRPTR args[ARG_COUNT] = {NULL, NULL, NULL, NULL};
    struct RDArgs *rda;

    rda = ReadArgs(ARG_STRING, (IPTR *)args, NULL);
    if (rda != NULL)
    {
        if (args[ARG_DAY] != NULL || args[ARG_DATE] != NULL || args[ARG_TIME] != NULL)
        {
            if ((error = setdate(args[ARG_DAY], args[ARG_DATE], args[ARG_TIME])) == RETURN_OK)
            {
                if (args[ARG_VER] != NULL)
                    printdate(args[ARG_VER]);
            }
        } else
            error = printdate(args[ARG_VER]);
        FreeArgs(rda);
    } else
    {
        PrintFault(IoErr(), "Date");
        error = RETURN_FAIL;
    }

    return(error);
}
