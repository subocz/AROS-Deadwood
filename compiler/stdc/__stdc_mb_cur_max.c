/*
    Copyright (C) 2012-2013, The AROS Development Team. All rights reserved.

    Get the MB_CUR_MAX value
    This function is both in static linklib as in stdc.library.
*/

int __stdc_mb_cur_max(void)
{
    return __aros_getbase_StdCBase()->mb_cur_max;
}
