/*
	Copyright (C) 1995-2001 AROS - The Amiga Research OS
	$Id$
 
	Desc: 
	Lang: english
*/

#ifndef MENUTASK_H
#define MENUTASK_H

#define MENUTASK_NAME "� Menu Handler �"
#define MENUTASK_STACKSIZE AROS_STACKSIZE
#ifdef __MORPHOS__
#define MENUTASK_PRIORITY 19
#else
#define MENUTASK_PRIORITY 0
#endif

#define ITEM_ITEM    1
#define ITEM_SUBITEM 2

#define AMIGAKEY_KEY_SPACING     4 /* GadTools assumes this, too */
#define AMIGAKEY_BORDER_SPACING  2

#ifdef SKINS
#include "intuition_intern.h"
#include "smallmenu.h"
#endif

/* Structure passed to the DefaultMenuHandler task when it's initialized */

struct MenuTaskParams
{
	struct IntuitionBase    *intuitionBase;
	struct Task             *Caller;
	struct MsgPort          *MenuHandlerPort; /* filled in by MenuHandler task */
	BOOL                     success;
};

#ifdef SKINS
struct MenuHandlerData
{
	struct Window             *win;
	struct Screen             *scr;
	struct IntDrawInfo        *dri;
	struct Menu               *menu;
	struct SmallMenuEntry     *entries;
	struct SmallMenuKeeper     rootsmk;
	struct IntuitionBase      *intuitionBase;
	struct Hook                backfillhook;
	struct HookData            hookdata;

	WORD                       scrmousex;
	WORD                       scrmousey;
	UWORD                      firstmenupick;
	UWORD                      lastmenupick;
	ULONG                      openseconds;
	ULONG                      openmicros;

	ULONG                      delayedopenseconds;
	ULONG                      delayedopenmicros;
	struct SmallMenuEntry     *delayedopen;
	
	BOOL                       active;
	BOOL                       keepmenuup;
	BOOL                       isundermouse;
#ifdef USEWINDOWLOCK
	BOOL                       windowlock;
#endif
};
#else

struct MenuHandlerData
{
	struct Window 	*win;
	struct Screen 	*scr;
	struct DrawInfo	*dri;
	struct Window	*menubarwin;
	struct Window 	*menuwin;
	struct Window	*submenuwin;
	struct Menu		*menu;
	struct Menu		*activemenu;
	struct MenuItem	*activeitem;
	struct MenuItem	*activesubitem;
	struct Rectangle	submenubox;
	struct Image	*checkmark;
	struct Image	*amigakey;
	WORD		menubarwidth;
	WORD		menubarheight;
	WORD		menubaritemwidth;
	WORD		menubaritemheight;
	WORD		nummenubaritems;
	WORD		activemenunum;
	WORD		activeitemnum;
	WORD		activesubitemnum;
	WORD		maxcommkeywidth_menu;
	WORD		maxcommkeywidth_submenu;
	WORD		scrmousex;
	WORD		scrmousey;
	UWORD		firstmenupick;
	UWORD		lastmenupick;
	BOOL 		active;
	BOOL		keepmenuup;
	ULONG		openseconds;
	ULONG		openmicros;
};

#endif

BOOL InitDefaultMenuHandler(struct IntuitionBase *IntuitionBase);
struct Task *CreateMenuHandlerTask(APTR taskparams, struct IntuitionBase *IntuitionBase);
void DefaultMenuHandler(struct MenuTaskParams *taskparams);
void AddToSelection(struct MenuHandlerData *mhd, struct SmallMenuEntry *entry,struct IntuitionBase *IntuitionBase);

BOOL HandleCheckItem(struct MenuHandlerData *mhd, struct SmallMenuEntry *entry,
							struct IntuitionBase *IntuitionBase);

#endif
