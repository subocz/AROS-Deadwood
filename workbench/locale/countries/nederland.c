/*
    Copyright (C) 1997 AROS - The Amiga Research OS
    $Id$

    Desc: Country data for Netherlands
    Lang:
*/

#include <exec/types.h>
#include <libraries/locale.h>
#include <libraries/iffparse.h>
#include <prefs/locale.h>

/* nederland.country: Lennard voor den Dag <ldp@worldonline.nl> */
struct CountryPrefs nederlandPrefs =
{
    /* Reserved */
    { 0, 0, 0, 0 },

    /* The country codes in the past have been rather inconsistant,
       sometimes they are 1 character, 2 chars or 3. It would be nice
       to have some consistency. Maybe use the 3 character name from
       ISO 3166? I (Iain) have a copy of the ISO3166 codes if anyone
       wants them...
    */

    /* Country code (left justify), telephone code, measuring system */
    MAKE_ID('N','E','D',0), 31, MS_ISO,

    /* Date time format, date format, time format */
    "%A %e %B %Y %q:%M",
    "%A %e %B %Y",
    "%q:%M:%S",

    /* Short datetime, short date, short time formats */
    "%e-%m-%y %q:%M",
    "%e-%m-%y",
    "%q:%M",

    /* Decimal point, group separator, frac group separator */
    ",", ".", "\x00",

    /* For grouping rules, see <libraries/locale.h> */

    /* Grouping, Frac Grouping */
    "\x03\x00", "\x00",

    /* Mon dec pt, mon group sep, mon frac group sep */
    ",", ".", "\x00",

    /* Mon Grouping, Mon frac grouping */
    "\x03\x00", "\x00",

    /* Mon Frac digits, Mon IntFrac digits, then number of digits in
       the fractional part of the money value. Most countries that
       use dollars and cents, would have 2 for this value

       (As would many of those who don't).
    */
    2, 2,

    /* Currency symbol, Small currency symbol */
    "fl", "c",

    /* Int CS, this is the ISO 4217 symbol, followed by the character to
       separate that symbol from the rest of the money. (\x00 for none).
    */
    "DFL\x00",

    /* Mon +ve sign, +ve space sep, +ve sign pos, +ve cs pos */
    "\x00", SS_SPACE, SP_PREC_ALL, CSP_PRECEDES,

    /* Mon -ve sign, -ve space sep, -ve sign pos, -ve cs pos */
    "-", SS_SPACE, SP_SUCC_CURR, CSP_PRECEDES,

    /* Calendar type */
    CT_7SUN
};
