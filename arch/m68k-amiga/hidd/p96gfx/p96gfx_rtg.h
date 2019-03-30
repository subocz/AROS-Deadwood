#ifndef P96GFX_P96RTG_H
#define P96GFX_P96RTG_H

/* Types for RGBFormat used */
typedef enum {
    RGBFB_NONE,		/* no valid RGB format (should not happen) */
    RGBFB_CLUT,		/* palette mode, set colors when opening screen using
			   tags or use SetRGB32/LoadRGB32(...) */
    RGBFB_R8G8B8,	/* TrueColor RGB (8 bit each) */
    RGBFB_B8G8R8,	/* TrueColor BGR (8 bit each) */
    RGBFB_R5G6B5PC,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: gggbbbbbrrrrrggg */
    RGBFB_R5G5B5PC,	/* HiColor15 (5 bit each), format: gggbbbbb0rrrrrgg */
    RGBFB_A8R8G8B8,	/* 4 Byte TrueColor ARGB (A unused alpha channel) */
    RGBFB_A8B8G8R8,	/* 4 Byte TrueColor ABGR (A unused alpha channel) */
    RGBFB_R8G8B8A8,	/* 4 Byte TrueColor RGBA (A unused alpha channel) */
    RGBFB_B8G8R8A8,	/* 4 Byte TrueColor BGRA (A unused alpha channel) */
    RGBFB_R5G6B5,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: rrrrrggggggbbbbb */
    RGBFB_R5G5B5,	/* HiColor15 (5 bit each), format: 0rrrrrgggggbbbbb */
    RGBFB_B5G6R5PC,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: gggrrrrrbbbbbggg */
    RGBFB_B5G5R5PC,	/* HiColor15 (5 bit each), format: gggrrrrr0bbbbbbgg */

    /* By now, the following formats are for use with a hardware window only
       (bitmap operations may be implemented incompletely) */

    RGBFB_Y4U2V2,	/* 2 Byte TrueColor YUV (CCIR recommendation CCIR601).
			   Each two-pixel unit is stored as one longword
			   containing luminance (Y) for each of the two pixels,
			   and chrominance (U,V) for alternate pixels.
			   The missing chrominance values are generated by
			   interpolation. (Y1-U0-Y0-V0) */
    RGBFB_Y4U1V1,	/* 1 Byte TrueColor ACCUPAK. Four adjacent pixels form
			   a packet of 5 bits Y (luminance) each pixel and 6 bits
			   U and V (chrominance) shared by the four pixels */

    RGBFB_MaxFormats
} RGBFTYPE;

#define RGBFF_NONE	(1<<RGBFB_NONE)
#define RGBFF_CLUT	(1<<RGBFB_CLUT)
#define RGBFF_R8G8B8	(1<<RGBFB_R8G8B8)
#define RGBFF_B8G8R8	(1<<RGBFB_B8G8R8)
#define RGBFF_R5G6B5PC	(1<<RGBFB_R5G6B5PC)
#define RGBFF_R5G5B5PC	(1<<RGBFB_R5G5B5PC)
#define RGBFF_A8R8G8B8	(1<<RGBFB_A8R8G8B8)
#define RGBFF_A8B8G8R8	(1<<RGBFB_A8B8G8R8)
#define RGBFF_R8G8B8A8	(1<<RGBFB_R8G8B8A8)
#define RGBFF_B8G8R8A8	(1<<RGBFB_B8G8R8A8)
#define RGBFF_R5G6B5	(1<<RGBFB_R5G6B5)
#define RGBFF_R5G5B5	(1<<RGBFB_R5G5B5)
#define RGBFF_B5G6R5PC	(1<<RGBFB_B5G6R5PC)
#define RGBFF_B5G5R5PC	(1<<RGBFB_B5G5R5PC)
#define RGBFF_Y4U2V2	(1<<RGBFB_Y4U2V2)
#define RGBFF_Y4U1V1	(1<<RGBFB_Y4U1V1)

#define RGBMASK_8BIT RGBFF_CLUT
#define RGBMASK_16BIT (RGBFF_R5G6B5PC | RGBFF_B5G6R5PC | RGBFF_R5G6B5)
#define RGBMASK_15BIT (RGBFF_R5G5B5PC | RGBFF_B5G5R5PC | RGBFF_R5G5B5)
#define RGBMASK_24BIT (RGBFF_R8G8B8 | RGBFF_B8G8R8)
#define RGBMASK_32BIT (RGBFF_A8R8G8B8 | RGBFF_A8B8G8R8 | RGBFF_R8G8B8A8 | RGBFF_B8G8R8A8)

#define	RGBFF_PLANAR	RGBFF_NONE
#define	RGBFF_CHUNKY	RGBFF_CLUT

#define	RGBFB_PLANAR	RGBFB_NONE
#define	RGBFB_CHUNKY	RGBFB_CLUT

#define RGBFB_SUPPORTMASK (RGBFF_CLUT | \
	RGBFF_A8R8G8B8 | RGBFF_A8B8G8R8 | RGBFF_R8G8B8A8 | RGBFF_B8G8R8A8 | RGBFF_B8G8R8 | RGBFF_R8G8B8 | \
	RGBFF_R5G6B5 | RGBFF_R5G5B5 | RGBFF_R5G6B5PC | RGBFF_R5G5B5PC | RGBFF_B5G6R5PC | RGBFF_B5G5R5PC)

/* BoardInfo hardare flags (set by the driver) */
#define BIB_HARDWARESPRITE		0		/* board has hardware sprite */
#define BIB_VBLANKINTERRUPT		4		/* board can cause a hardware interrupt on a vertical retrace */
#define BIB_BLITTER				15		/* board has blitter */
/* BoardInfo user flags (set by the rtg subsystem) */
#define BIB_HIRESSPRITE			16		/* mouse sprite has double resolution */
#define BIB_BIGSPRITE			17		/* user wants big mouse sprite */
#define BIB_NOBLITTER			24		/* disable all blitter functions */

#define BIF_HARDWARESPRITE		(1 << BIB_HARDWARESPRITE)
#define BIF_VBLANKINTERRUPT		(1 << BIB_VBLANKINTERRUPT)
#define BIF_BLITTER				(1 << BIB_BLITTER)
#define BIF_HIRESSPRITE			(1 << BIB_HIRESSPRITE)
#define BIF_BIGSPRITE			(1 << BIB_BIGSPRITE)
#define BIF_NOBLITTER			(1 << BIB_NOBLITTER)

enum {
    PLANAR,
    CHUNKY,
    HICOLOR,
    TRUECOLOR,
    TRUEALPHA,
    MAXMODES
};

#define MAXRESOLUTIONNAMELENGTH 22

struct LibResolution {
	struct Node node;
    char P96ID[6];
    char Name[MAXRESOLUTIONNAMELENGTH];
    ULONG DisplayID;
    UWORD Width;
    UWORD Height;
    UWORD Flags;
    struct ModeInfo *Modes[MAXMODES];
    APTR BoardInfo;
};

struct RenderInfo {
	APTR			Memory;
	WORD			BytesPerRow;
	WORD			pad;
	RGBFTYPE		RGBFormat;
};

struct CLUTEntry {
	UBYTE	Red;
	UBYTE	Green;
	UBYTE	Blue;
};

struct Template {
	APTR			Memory;
	WORD			BytesPerRow;
	UBYTE			XOffset;
	UBYTE			DrawMode;
	ULONG			FgPen;
	ULONG			BgPen;
};

struct Pattern {
	APTR			Memory;
	UWORD			XOffset, YOffset;
	ULONG			FgPen, BgPen;
	UBYTE			Size;
	UBYTE			DrawMode;
};

struct Line {
	WORD            X, Y;
	UWORD           Length;
	WORD            dX, dY;
	WORD            sDelta, lDelta, twoSDminusLD;
	UWORD           LinePtrn;
	UWORD           PatternShift;
	ULONG           FgPen, BgPen;
	BOOL            Horizontal;
	UBYTE           DrawMode;
	BYTE            pad;
	UWORD           Xorigin, Yorigin;
};

struct ModeInfo {
	struct Node	Node;
	WORD			OpenCount;
	BOOL			Active;
	UWORD			Width;
	UWORD			Height;
	UBYTE			Depth;
	UBYTE			Flags;
	UWORD			HorTotal;
	UWORD			HorBlankSize;
	UWORD			HorSyncStart;
	UWORD			HorSyncSize;
	UBYTE			HorSyncSkew;
	UBYTE			HorEnableSkew;
	UWORD			VerTotal;
	UWORD			VerBlankSize;
	UWORD			VerSyncStart;
	UWORD			VerSyncSize;
	union{
		UBYTE		Clock;
		UBYTE		Numerator;
	};
	union{
		UBYTE		ClockDivide;
		UBYTE		Denominator;
	};
	ULONG			PixelClock;
};


#define GMB_DOUBLECLOCK		0
#define GMB_INTERLACE		1
#define GMB_DOUBLESCAN		2
#define GMB_HPOLARITY		3
#define GMB_VPOLARITY		4
#define GMB_COMPATVIDEO		5
#define GMB_DOUBLEVERTICAL	6
#define GMF_DOUBLECLOCK		(1L<<GMB_DOUBLECLOCK)
#define GMF_INTERLACE		(1L<<GMB_INTERLACE)
#define GMF_DOUBLESCAN		(1L<<GMB_DOUBLESCAN)
#define GMF_HPOLARITY		(1L<<GMB_HPOLARITY)
#define GMF_VPOLARITY		(1L<<GMB_VPOLARITY)
#define GMF_COMPATVIDEO		(1L<<GMB_COMPATVIDEO)
#define GMF_DOUBLEVERTICAL	(1L<<GMB_DOUBLEVERTICAL)

#define PSSO_BitMapExtra_BoardNode        0
#define PSSO_BitMapExtra_HashChain        8 /* BoardNode is 8-bytes */
#define PSSO_BitMapExtra_Match           12
#define PSSO_BitMapExtra_BitMap          16
#define PSSO_BitMapExtra_BoardInfo       20
#define PSSO_BitMapExtra_MemChunk        24
#define PSSO_BitMapExtra_RenderInfo      28
#define PSSO_BitMapExtra_Width           40 /* RenderInfo is 12-bytes */
#define PSSO_BitMapExtra_Height          42
#define PSSO_BitMapExtra_Flags           44
#define PSSO_BitMapExtra_BaseLevel       46
#define PSSO_BitMapExtra_CurrentLevel    48
#define PSSO_BitMapExtra_CompanionMaster 50
#define PSSO_BitMapExtra_Last            54

#define PSSO_BoardInfo_RegisterBase		    0
#define PSSO_BoardInfo_MemoryBase		    PSSO_BoardInfo_RegisterBase + 4
#define PSSO_BoardInfo_MemoryIOBase		    PSSO_BoardInfo_MemoryBase + 4
#define PSSO_BoardInfo_MemorySize		    PSSO_BoardInfo_MemoryIOBase + 4
#define PSSO_BoardInfo_BoardName		    PSSO_BoardInfo_MemorySize + 4
#define PSSO_BoardInfo_VBIName			    PSSO_BoardInfo_BoardName + 4
#define PSSO_BoardInfo_CardBase			    PSSO_BoardInfo_VBIName + 32
#define PSSO_BoardInfo_ChipBase			    PSSO_BoardInfo_CardBase + 4
#define PSSO_BoardInfo_ExecBase			    PSSO_BoardInfo_ChipBase + 4
#define PSSO_BoardInfo_UtilBase			    PSSO_BoardInfo_ExecBase + 4
#define PSSO_BoardInfo_HardInterrupt		    PSSO_BoardInfo_UtilBase + 4
#define PSSO_BoardInfo_SoftInterrupt		    PSSO_BoardInfo_HardInterrupt + 22 /* The HardInterrupt is 22-bytes */
#define PSSO_BoardInfo_BoardLock		    PSSO_BoardInfo_SoftInterrupt + 22 /* The SoftInterrupt is 22-bytes */
#define PSSO_BoardInfo_ResolutionsList		    PSSO_BoardInfo_BoardLock + 46 /* On the BoardLock, we were having some fun... */
#define PSSO_BoardInfo_BoardType		    PSSO_BoardInfo_ResolutionsList + 12 /* The ResolutionsList is 12-bytes */
#define PSSO_BoardInfo_PaletteChipType		    PSSO_BoardInfo_BoardType + 4
#define PSSO_BoardInfo_GraphicsControllerType	    PSSO_BoardInfo_PaletteChipType + 4
#define PSSO_BoardInfo_MoniSwitch		    PSSO_BoardInfo_GraphicsControllerType + 4
#define PSSO_BoardInfo_BitsPerCannon		    PSSO_BoardInfo_MoniSwitch + 2
#define PSSO_BoardInfo_Flags			    PSSO_BoardInfo_BitsPerCannon + 2
#define PSSO_BoardInfo_SoftSpriteFlags		    PSSO_BoardInfo_Flags + 4
#define PSSO_BoardInfo_ChipFlags		    PSSO_BoardInfo_SoftSpriteFlags + 2
#define PSSO_BoardInfo_CardFlags		    PSSO_BoardInfo_ChipFlags + 2
#define PSSO_BoardInfo_BoardNum			    PSSO_BoardInfo_CardFlags + 4
#define PSSO_BoardInfo_RGBFormats		    PSSO_BoardInfo_BoardNum + 2
#define PSSO_BoardInfo_MaxHorValue		    PSSO_BoardInfo_RGBFormats + 2
#define PSSO_BoardInfo_MaxVerValue		    PSSO_BoardInfo_MaxHorValue + MAXMODES * 2
#define PSSO_BoardInfo_MaxHorResolution		    PSSO_BoardInfo_MaxVerValue + MAXMODES * 2
#define PSSO_BoardInfo_MaxVerResolution		    PSSO_BoardInfo_MaxHorResolution + MAXMODES * 2
#define PSSO_BoardInfo_MaxMemorySize		    PSSO_BoardInfo_MaxVerResolution + MAXMODES * 2
#define PSSO_BoardInfo_MaxChunkSize		    PSSO_BoardInfo_MaxMemorySize + 4
#define PSSO_BoardInfo_MemoryClock		    PSSO_BoardInfo_MaxChunkSize + 4
#define PSSO_BoardInfo_PixelClockCount		    PSSO_BoardInfo_MemoryClock + 4

#define PSSO_BoardInfo_AllocCardMem		    PSSO_BoardInfo_PixelClockCount + MAXMODES * 4
#define PSSO_BoardInfo_FreeCardMem		    PSSO_BoardInfo_AllocCardMem + 4

#define PSSO_BoardInfo_SetSwitch		    PSSO_BoardInfo_FreeCardMem + 4

#define PSSO_BoardInfo_SetColorArray		    PSSO_BoardInfo_SetSwitch + 4

#define PSSO_BoardInfo_SetDAC			    PSSO_BoardInfo_SetColorArray + 4
#define PSSO_BoardInfo_SetGC			    PSSO_BoardInfo_SetDAC + 4
#define PSSO_BoardInfo_SetPanning		    PSSO_BoardInfo_SetGC + 4
#define PSSO_BoardInfo_CalculateBytesPerRow	    PSSO_BoardInfo_SetPanning + 4
#define PSSO_BoardInfo_CalculateMemory		    PSSO_BoardInfo_CalculateBytesPerRow + 4
#define PSSO_BoardInfo_GetCompatibleFormats	    PSSO_BoardInfo_CalculateMemory + 4
#define PSSO_BoardInfo_SetDisplay		    PSSO_BoardInfo_GetCompatibleFormats + 4

#define PSSO_BoardInfo_ResolvePixelClock	    PSSO_BoardInfo_SetDisplay + 4
#define PSSO_BoardInfo_GetPixelClock		    PSSO_BoardInfo_ResolvePixelClock + 4
#define PSSO_BoardInfo_SetClock			    PSSO_BoardInfo_GetPixelClock + 4

#define PSSO_BoardInfo_SetMemoryMode		    PSSO_BoardInfo_SetClock + 4
#define PSSO_BoardInfo_SetWriteMask		    PSSO_BoardInfo_SetMemoryMode + 4
#define PSSO_BoardInfo_SetClearMask		    PSSO_BoardInfo_SetWriteMask + 4
#define PSSO_BoardInfo_SetReadPlane		    PSSO_BoardInfo_SetClearMask + 4

#define PSSO_BoardInfo_WaitVerticalSync		    PSSO_BoardInfo_SetReadPlane + 4
#define PSSO_BoardInfo_SetInterrupt		    PSSO_BoardInfo_WaitVerticalSync + 4

#define PSSO_BoardInfo_WaitBlitter		    PSSO_BoardInfo_SetInterrupt + 4

#define PSSO_BoardInfo_ScrollPlanar		    PSSO_BoardInfo_WaitBlitter + 4
#define PSSO_BoardInfo_ScrollPlanarDefault	    PSSO_BoardInfo_ScrollPlanar + 4
#define PSSO_BoardInfo_UpdatePlanar		    PSSO_BoardInfo_ScrollPlanarDefault + 4
#define PSSO_BoardInfo_UpdatePlanarDefault	    PSSO_BoardInfo_UpdatePlanar + 4
#define PSSO_BoardInfo_BlitPlanar2Chunky	    PSSO_BoardInfo_UpdatePlanarDefault + 4
#define PSSO_BoardInfo_BlitPlanar2ChunkyDefault	    PSSO_BoardInfo_BlitPlanar2Chunky + 4

#define PSSO_BoardInfo_FillRect			    PSSO_BoardInfo_BlitPlanar2ChunkyDefault + 4
#define PSSO_BoardInfo_FillRectDefault		    PSSO_BoardInfo_FillRect + 4
#define PSSO_BoardInfo_InvertRect		    PSSO_BoardInfo_FillRectDefault + 4
#define PSSO_BoardInfo_InvertRectDefault	    PSSO_BoardInfo_InvertRect + 4
#define PSSO_BoardInfo_BlitRect			    PSSO_BoardInfo_InvertRectDefault + 4
#define PSSO_BoardInfo_BlitRectDefault		    PSSO_BoardInfo_BlitRect + 4
#define PSSO_BoardInfo_BlitTemplate		    PSSO_BoardInfo_BlitRectDefault + 4
#define PSSO_BoardInfo_BlitTemplateDefault	    PSSO_BoardInfo_BlitTemplate + 4
#define PSSO_BoardInfo_BlitPattern		    PSSO_BoardInfo_BlitTemplateDefault + 4
#define PSSO_BoardInfo_BlitPatternDefault	    PSSO_BoardInfo_BlitPattern + 4
#define PSSO_BoardInfo_DrawLine			    PSSO_BoardInfo_BlitPatternDefault + 4
#define PSSO_BoardInfo_DrawLineDefault		    PSSO_BoardInfo_DrawLine + 4
#define PSSO_BoardInfo_BlitRectNoMaskComplete	    PSSO_BoardInfo_DrawLineDefault + 4
#define PSSO_BoardInfo_BlitRectNoMaskCompleteDefault PSSO_BoardInfo_BlitRectNoMaskComplete + 4
#define PSSO_BoardInfo_BlitPlanar2Direct	    PSSO_BoardInfo_BlitRectNoMaskCompleteDefault + 4
#define PSSO_BoardInfo_BlitPlanar2DirectDefault	    PSSO_BoardInfo_BlitPlanar2Direct + 4

#define PSSO_BoardInfo_Reserved0		    PSSO_BoardInfo_BlitPlanar2DirectDefault + 4
#define PSSO_BoardInfo_Reserved0Default		    PSSO_BoardInfo_Reserved0 + 4
#define PSSO_BoardInfo_Reserved1		    PSSO_BoardInfo_Reserved0Default + 4
#define PSSO_BoardInfo_Reserved1Default		    PSSO_BoardInfo_Reserved1 + 4
#define PSSO_BoardInfo_Reserved2		    PSSO_BoardInfo_Reserved1Default + 4
#define PSSO_BoardInfo_Reserved2Default		    PSSO_BoardInfo_Reserved2 + 4
#define PSSO_BoardInfo_Reserved3		    PSSO_BoardInfo_Reserved2Default + 4
#define PSSO_BoardInfo_Reserved3Default		    PSSO_BoardInfo_Reserved3 + 4
#define PSSO_BoardInfo_Reserved4		    PSSO_BoardInfo_Reserved3Default + 4
#define PSSO_BoardInfo_Reserved4Default		    PSSO_BoardInfo_Reserved4 + 4
#define PSSO_BoardInfo_Reserved5		    PSSO_BoardInfo_Reserved4Default + 4
#define PSSO_BoardInfo_Reserved5Default		    PSSO_BoardInfo_Reserved5 + 4

#define PSSO_BoardInfo_SetDPMSLevel		    PSSO_BoardInfo_Reserved5Default + 4
#define PSSO_BoardInfo_ResetChip		    PSSO_BoardInfo_SetDPMSLevel + 4

#define PSSO_BoardInfo_GetFeatureAttrs		    PSSO_BoardInfo_ResetChip + 4

#define PSSO_BoardInfo_AllocBitMap		    PSSO_BoardInfo_GetFeatureAttrs + 4
#define PSSO_BoardInfo_FreeBitMap		    PSSO_BoardInfo_AllocBitMap + 4
#define PSSO_BoardInfo_GetBitMapAttr		    PSSO_BoardInfo_FreeBitMap + 4

#define PSSO_BoardInfo_SetSprite		    PSSO_BoardInfo_GetBitMapAttr + 4
#define PSSO_BoardInfo_SetSpritePosition	    PSSO_BoardInfo_SetSprite + 4
#define PSSO_BoardInfo_SetSpriteImage		    PSSO_BoardInfo_SetSpritePosition + 4
#define PSSO_BoardInfo_SetSpriteColor		    PSSO_BoardInfo_SetSpriteImage + 4

#define PSSO_BoardInfo_CreateFeature		    PSSO_BoardInfo_SetSpriteColor + 4
#define PSSO_BoardInfo_SetFeatureAttrs		    PSSO_BoardInfo_CreateFeature + 4
#define PSSO_BoardInfo_DeleteFeature		    PSSO_BoardInfo_SetFeatureAttrs + 4
#define PSSO_BoardInfo_SpecialFeatures		    PSSO_BoardInfo_DeleteFeature + 4

#define PSSO_BoardInfo_ModeInfo			    PSSO_BoardInfo_SpecialFeatures + 12 /* SpecialFeatures is 12-bytes */
#define PSSO_BoardInfo_RGBFormat		    PSSO_BoardInfo_ModeInfo + 4
#define PSSO_BoardInfo_XOffset			    PSSO_BoardInfo_RGBFormat + 4
#define PSSO_BoardInfo_YOffset			    PSSO_BoardInfo_XOffset + 2
#define PSSO_BoardInfo_Depth			    PSSO_BoardInfo_YOffset + 2
#define PSSO_BoardInfo_ClearMask		    PSSO_BoardInfo_Depth + 1
#define PSSO_BoardInfo_Border			    PSSO_BoardInfo_ClearMask + 1
#define PSSO_BoardInfo_Mask			    PSSO_BoardInfo_Border + 2 /* BOOL type is only 2-bytes! */
#define PSSO_BoardInfo_CLUT			    PSSO_BoardInfo_Mask + 4
#define PSSO_BoardInfo_ViewPort			    PSSO_BoardInfo_CLUT + 3*256
#define PSSO_BoardInfo_VisibleBitMap		    PSSO_BoardInfo_ViewPort + 4
#define PSSO_BoardInfo_BitMapExtra		    PSSO_BoardInfo_VisibleBitMap + 4
#define PSSO_BoardInfo_BitMapList		    PSSO_BoardInfo_BitMapExtra + 4
#define PSSO_BoardInfo_MemList			    PSSO_BoardInfo_BitMapList + 12 /* BitMapList is 12-bytes */
#define PSSO_BoardInfo_MouseX			    PSSO_BoardInfo_MemList + 12 /* MemList is 12-bytes */
#define PSSO_BoardInfo_MouseY			    PSSO_BoardInfo_MouseX + 2
#define PSSO_BoardInfo_MouseWidth		    PSSO_BoardInfo_MouseY + 2
#define PSSO_BoardInfo_MouseHeight		    PSSO_BoardInfo_MouseWidth + 1
#define PSSO_BoardInfo_MouseXOffset		    PSSO_BoardInfo_MouseHeight + 1
#define PSSO_BoardInfo_MouseYOffset		    PSSO_BoardInfo_MouseXOffset + 1
#define PSSO_BoardInfo_MouseImage		    PSSO_BoardInfo_MouseYOffset + 1
#define PSSO_BoardInfo_MousePens		    PSSO_BoardInfo_MouseImage + 4
#define PSSO_BoardInfo_MouseRect		    PSSO_BoardInfo_MousePens + 4
#define PSSO_BoardInfo_MouseChunky		    PSSO_BoardInfo_MouseRect + 8 /* MouseRect is 8-bytes */
#define PSSO_BoardInfo_MouseRendered		    PSSO_BoardInfo_MouseChunky + 4
#define PSSO_BoardInfo_MouseSaveBuffer		    PSSO_BoardInfo_MouseRendered + 4

#define PSSO_BoardInfo_ChipData			    PSSO_BoardInfo_MouseSaveBuffer + 4
#define PSSO_BoardInfo_CardData			    PSSO_BoardInfo_ChipData + 16 * 4
#define PSSO_BoardInfo_MemorySpaceBase		    PSSO_BoardInfo_CardData + 16 * 4
#define PSSO_BoardInfo_MemorySpaceSize		    PSSO_BoardInfo_MemorySpaceBase + 4
#define PSSO_BoardInfo_DoubleBufferList		    PSSO_BoardInfo_MemorySpaceSize + 4
#define PSSO_BoardInfo_SyncTime			    PSSO_BoardInfo_DoubleBufferList + 4
#define PSSO_BoardInfo_SyncPeriod		    PSSO_BoardInfo_SyncTime + 4
#define PSSO_BoardInfo_SoftVBlankPort		    PSSO_BoardInfo_SyncPeriod + 8
#define PSSO_BoardInfo_WaitQ		            PSSO_BoardInfo_SyncPeriod + 34
#define PSSO_BoardInfo_AROSFlag			    PSSO_BoardInfo_WaitQ + 12
#define PSSO_BoardInfo_SizeOf			    PSSO_BoardInfo_AROSFlag + 4

#define P96BoardType_PicassoII                      6
#define P96BoardType_Piccolo                        7
#define P96BoardType_RetinaBLT                      8
#define P96BoardType_PicassoIV                      10
#define P96BoardType_PiccoloSD64                    11
#define P96BoardType_A2410                          12
#define P96BoardType_Pixel64                        13
#define P96BoardType_UAEGfx                         14
#define P96BoardType_Vampire                        25

/* inline access wrappers */
static inline APTR gp(UBYTE *p)
{
    return ((APTR*)p)[0];
}
static inline ULONG gl(UBYTE *p)
{
    return ((ULONG*)p)[0];
}
static inline UWORD gw(UBYTE *p)
{
    return ((UWORD*)p)[0];
}
static inline void pp(UBYTE *p, APTR a)
{
    ((APTR*)p)[0] = a;
}
static inline void pl(UBYTE *p, ULONG l)
{
    ((ULONG*)p)[0] = l;
}
static inline void pw(UBYTE *p, WORD w)
{
    ((WORD*)p)[0] = w;
}
static inline void pb(UBYTE *p, BYTE b)
{
    ((BYTE*)p)[0] = b;
}

/* RTG Support functions */
extern const UBYTE modetable[16];

WORD getrtgdepth (ULONG rgbformat);
ULONG getrtgformat(struct p96gfx_staticdata *csd, OOP_Object *);
void makerenderinfo(struct p96gfx_staticdata *csd, struct RenderInfo*, struct bm_data*);
struct ModeInfo *getrtgmodeinfo(struct p96gfx_staticdata *csd, OOP_Object *sync, OOP_Object *pixfmt, struct ModeInfo *modeinfo);
void InitRTG(APTR boardinfo);

/* Card functions */
BOOL FindCard(struct p96gfx_staticdata *csd);
BOOL InitCard(struct p96gfx_staticdata *csd);
BOOL SetDisplay(struct p96gfx_staticdata *csd, BOOL state);
BOOL SetSwitch(struct p96gfx_staticdata *csd, BOOL state);
void SetColorArray(struct p96gfx_staticdata *csd, UWORD start, UWORD count);
void SetDAC(struct p96gfx_staticdata *csd);
void SetGC(struct p96gfx_staticdata *csd, struct ModeInfo *mi, BOOL border);
void SetPanning(struct p96gfx_staticdata *csd, UBYTE *video, UWORD width, WORD x, WORD y);
/*
TODO:
SetDPMSLevel
GetFeatureAttrs
SetFeatureAttrs
*/

/* Card Render Operation Stubs .. */
BOOL DrawLine(struct p96gfx_staticdata *csd, struct RenderInfo *ri,
    struct Line *line, ULONG rgbformat);
BOOL BlitRect(struct p96gfx_staticdata *csd, struct RenderInfo *ri,
    WORD sx, WORD sy, WORD dx, WORD dy, WORD w, WORD h, UBYTE mask, ULONG rgbformat);
BOOL FillRect(struct p96gfx_staticdata *csd, struct RenderInfo *ri, WORD x, WORD y, WORD w, WORD h, ULONG pen, UBYTE mask, ULONG rgbformat);
BOOL InvertRect(struct p96gfx_staticdata *csd, struct RenderInfo *ri, WORD x, WORD y, WORD w, WORD h, UBYTE mask, ULONG rgbformat);
BOOL BlitRectNoMaskComplete(struct p96gfx_staticdata *csd, struct RenderInfo *risrc, struct RenderInfo *ridst,
    WORD sx, WORD sy, WORD dx, WORD dy, WORD w, WORD h, UBYTE opcode, ULONG rgbformat);
BOOL BlitPattern(struct p96gfx_staticdata *csd, struct RenderInfo *ri, struct Pattern *pat,
    WORD x, WORD y, WORD w, WORD h, UBYTE mask, ULONG rgbformat);
BOOL BlitTemplate(struct p96gfx_staticdata *csd, struct RenderInfo *ri, struct Template *tmpl,
    WORD x, WORD y, WORD w, WORD h, UBYTE mask, ULONG rgbformat);
/*
TODO:
BlitPlanar2Chunky
BlitPlanar2Direct
WriteYUVRect
*/

WORD CalculateBytesPerRow(struct p96gfx_staticdata *csd, WORD width, ULONG rgbformat);
BOOL SetSprite(struct p96gfx_staticdata *sd, BOOL activate);
BOOL SetSpritePosition(struct p96gfx_staticdata *sd);
BOOL SetSpriteImage(struct p96gfx_staticdata *sd);
BOOL SetSpriteColor(struct p96gfx_staticdata *sd, UBYTE idx, UBYTE r, UBYTE g, UBYTE b);

/* Real RTG only card functions */
ULONG GetPixelClock(struct p96gfx_staticdata *csd, struct ModeInfo *mi, ULONG index, ULONG rgbformat);
ULONG ResolvePixelClock(struct p96gfx_staticdata *csd, struct ModeInfo *mi, ULONG pixelclock, ULONG rgbformat);
ULONG SetClock(struct p96gfx_staticdata *csd);
void SetMemoryMode(struct p96gfx_staticdata *csd, ULONG rgbformat);
void WaitBlitter(struct p96gfx_staticdata *csd);
void SetInterrupt(struct p96gfx_staticdata *csd, ULONG state);
/*
TODO:
GetVSyncState
WaitVerticalSync
GetVBeamPos
*/
#endif
