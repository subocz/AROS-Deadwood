#include <proto/exec.h>
#include <proto/aros.h>
#include <proto/dos.h>
#include <proto/mathffp.h>

#include <stdio.h>

#include <exec/types.h>

struct Library * MathBase;

int main(int argc, char ** argv)
{
  if (!(MathBase = OpenLibrary("mathffp.library", 0L)))
  {
    FPrintf((BPTR)stderr, "Couldn't open mathffp.library\n");
    return (0);
  }

  Printf("Basic mathffp functionality test...\n");

  /* this should set the zero-bit*/
  if ( 0 != SPAbs(0))
     Printf("Error with the SPAbs-function!\n");
  else
     Printf("SPAbs-function ok!\n");

  CloseLibrary(MathBase);
  return (0);
}
