/*
    (C) 1995-96 AROS - The Amiga Replacement OS
    $Id$

    Desc: Read an icon from an .info file
    Lang: english
*/
#include <stdio.h>

#include <aros/bigendianio.h>
#include <graphics/gfxbase.h>
#include <workbench/workbench.h>
#include <workbench/icon.h>

#include <proto/icon.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>

#define IM(x)       ((struct Image *)x)

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
struct Library * IconBase;

void DoWindow (struct DiskObject * dobj)
{
    struct Window   * win = NULL;
    struct RastPort * rp;
    struct IntuiMessage * im;
    int cont;

    GfxBase=(struct GfxBase *)OpenLibrary(GRAPHICSNAME,39);
    IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library",39);

    if (!GfxBase)
    {
	fprintf (stderr, "Couldn't open %s\n", GRAPHICSNAME);
	goto end;
    }

    if (!IntuitionBase)
    {
	fprintf (stderr, "Couldn't open intuition.library\n");
	goto end;
    }

    dobj->do_Gadget.LeftEdge = dobj->do_Gadget.Width * 2 + 30;
    dobj->do_Gadget.TopEdge  = 10;

    win = OpenWindowTags (NULL
	, WA_Title,	    "Show an icon"
	, WA_Left,	    100
	, WA_Top,	    100
	, WA_Width,	    dobj->do_Gadget.Width * 3 + 40
	, WA_Height,	    dobj->do_Gadget.Height + 20
	, WA_IDCMP,	    IDCMP_RAWKEY
	, WA_SimpleRefresh, TRUE
	, WA_Gadgets,	    &dobj->do_Gadget
	, TAG_END
    );

    if (!win)
	goto end;

    rp = win->RPort;

    DrawImage (rp, dobj->do_Gadget.GadgetRender, 10, 10);
    DrawImage (rp, dobj->do_Gadget.SelectRender, 20 + dobj->do_Gadget.Width, 10);

    cont = 1;

    printf ("Press a key to exit\n");

    while (cont)
    {
	if ((im = (struct IntuiMessage *)GetMsg (win->UserPort)))
	{
	    /* D("Got msg\n"); */
	    switch (im->Class)
	    {
	    case IDCMP_RAWKEY:
		cont = FALSE;
		break;

	    }

	    ReplyMsg ((struct Message *)im);
	}
	else
	{
	    /* D("Waiting\n"); */
	    Wait (1L << win->UserPort->mp_SigBit);
	}
    }

end:
    if (win)
	CloseWindow (win);

    if (GfxBase)
	CloseLibrary ((struct Library *)GfxBase);

    if (IntuitionBase)
	CloseLibrary ((struct Library *)IntuitionBase);

    return;
} /* DoWindow */

int main (int argc, char ** argv)
{
    struct RDArgs     * rda;
    struct DiskObject * dobj;

    STRPTR arg;
    int    rc;
    int    t;

    rc = 0;

    IconBase = OpenLibrary (ICONNAME, 39);

    if (!IconBase)
    {
	fprintf (stderr, "Couldn't open %s\n", ICONNAME);
	return RETURN_FAIL;
    }

    rda = ReadArgs ("IconFile/A", (IPTR *)&arg, NULL);

    if (rda)
    {
	if (!(dobj = GetDiskObject (arg)) )
	{
	    fprintf (stderr, "Cannot open icon for %s: ", arg);
	    PrintFault (IoErr(), "");
	    rc = 10;
	}
	else
	{
	    /* hexdump (dobj, 0L, sizeof (struct DiskObject)); */

	    printf ("Some information about the icon:\n"
		"Magic = %d\n"
		"Version = %d\n"
		"Type = %d\n"
		"Gadget: %dx%d+%d+%d Flags=%x Act=%x Type=%d\n"
		"Stack = %ld\n"
		, dobj->do_Magic
		, dobj->do_Version
		, dobj->do_Type
		, dobj->do_Gadget.Width
		, dobj->do_Gadget.Height
		, dobj->do_Gadget.LeftEdge
		, dobj->do_Gadget.TopEdge
		, dobj->do_Gadget.Flags
		, dobj->do_Gadget.Activation
		, dobj->do_Gadget.GadgetType
		, dobj->do_StackSize
	    );

	    if (dobj->do_Gadget.GadgetRender)
	    {
		printf ("GImage: %dx%d+%d+%d\n"
		    , IM(dobj->do_Gadget.GadgetRender)->Width
		    , IM(dobj->do_Gadget.GadgetRender)->Height
		    , IM(dobj->do_Gadget.GadgetRender)->LeftEdge
		    , IM(dobj->do_Gadget.GadgetRender)->TopEdge
		);

		/* hexdump (IM(dobj->do_Gadget.GadgetRender)->ImageData
		    , 0L
		    , 720
		); */
	    }
	    else
	    {
		printf ("GImage: none\n");
	    }

	    if (dobj->do_Gadget.SelectRender)
	    {
		printf ("SImage: %dx%d+%d+%d\n"
		    , IM(dobj->do_Gadget.SelectRender)->Width
		    , IM(dobj->do_Gadget.SelectRender)->Height
		    , IM(dobj->do_Gadget.SelectRender)->LeftEdge
		    , IM(dobj->do_Gadget.SelectRender)->TopEdge
		);

		/* hexdump (IM(dobj->do_Gadget.SelectRender)->ImageData
		    , 0L
		    , 720
		); */
	    }
	    else
	    {
		printf ("SImage: none\n");
	    }

	    printf ("DefaultTool: %s\n", dobj->do_DefaultTool);

	    printf ("ToolTypes:\n");

	    if (dobj->do_ToolTypes)
		for (t=0; dobj->do_ToolTypes[t]; t++)
		    printf ("TT %d: %s\n", t, dobj->do_ToolTypes[t]);
	    else
		printf ("--- none ---\n");

	    if (!PutDiskObject ("readicon", dobj))
		PrintFault (IoErr(), "Writing of icon to readicon.info failed");

	    DoWindow (dobj);

	    FreeDiskObject (dobj);
	}

	FreeArgs (rda);
    }
    else
	rc = 10;

    return rc;
} /* main */

