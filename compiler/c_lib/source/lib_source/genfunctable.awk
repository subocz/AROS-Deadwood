BEGIN {
    maxlvo = 0;
    stderr="/dev/stderr";

    file = "libdefs.h";

    while ((getline < file) > 0)
    {
	if ($2 == "BASENAME")
	{
	    lib = $3;
	    basename = $3;
	}
	else if ($2 == "LIBBASE")
	{
	    libbase = $3;
	}
    }

    print "/*";
    print "    (C) 1995-96 AROS - The Amiga Replacement OS";
    print "    *** Automatic generated file. Do not edit ***";
    print "    Desc: Function table for " lib;
    print "    Lang: english";
    print "*/";
    print "#ifndef LIBCORE_COMPILER_H\n#   include <libcore/compiler.h>\n#endif";
    print "#ifndef NULL\n#define NULL ((void *)0)\n#endif\n";
    print "#include \"libdefs.h\"";

    f[1] = "open";
    f[2] = "close";
    f[3] = "expunge";
    f[4] = "null";

    verbose_pattern = libbase"[ \\t]*,[ \\t]*[0-9]+[ \\t]*,[ \\t]*"basename;

#print verbose_pattern > "/dev/stderr";

    if (maxlvo < 4)
	maxlvo = 4;
}
/AROS_LH[0-9]/ {
    line=$0;
    sub(/[ \t]*$/,"",line);
    if (match(line,/[a-zA-Z0-9_]+,$/))
    {
	name=substr(line,RSTART,RLENGTH-1);
#print "/* FOUND " name " */";
    }
}
/LIBBASE[ \t]*,[ \t]*[0-9]+/ || $0 ~ verbose_pattern {
#print "/* LOC " $0 " */"
    match ($0, /,[ \t]*[0-9]+/);
    line=substr($0,RSTART,RLENGTH);
#print "line2="line
    sub (/,[ \t]*/,"",line);
#print "line3="line
    lvo=int(line);

#print "lvo="lvo;

    if (f[lvo] != "")
	printf "Error: lvo "lvo" is used by "f[lvo]" and "name >> stderr;

    f[lvo] = name;
    if (lvo > maxlvo)
	maxlvo = lvo;
}
/^\/\*AROS/ {
    if ($2 == "alias") {
	a[lvo] = $3;
	f[lvo] = $4;
    }
}
END {
    print "extern void AROS_SLIB_ENTRY(LC_BUILDNAME(OpenLib),LibHeader) (void);";
    print "extern void AROS_SLIB_ENTRY(LC_BUILDNAME(CloseLib),LibHeader) (void);";
    print "extern void AROS_SLIB_ENTRY(LC_BUILDNAME(ExpungeLib),LibHeader) (void);";
    print "extern void AROS_SLIB_ENTRY(LC_BUILDNAME(ExtFuncLib),LibHeader) (void);";

    for (t=5; t<=maxlvo; t++)
    {
	if (t in f && !(t in a))
	    print "extern void AROS_SLIB_ENTRY(" f[t] ",BASENAME) (void);";
    }

    print "\nvoid *const LIBFUNCTABLE[]=\n{";

    show=0;

    print "    AROS_SLIB_ENTRY(LC_BUILDNAME(OpenLib),LibHeader),";
    print "    AROS_SLIB_ENTRY(LC_BUILDNAME(CloseLib),LibHeader),";
    print "    AROS_SLIB_ENTRY(LC_BUILDNAME(ExpungeLib),LibHeader),";
    print "    AROS_SLIB_ENTRY(LC_BUILDNAME(ExtFuncLib),LibHeader),";

    if (maxlvo <= 4)
    {
	print "Error: No matching functions found" > "/dev/stderr";
	exit (10);
    }

    for (t=5; t<=maxlvo; t++)
    {
	line="    ";

	if (t in f)
	{
	    line=line "AROS_SLIB_ENTRY(" f[t] ",BASENAME),";

	    if (t in a)
		line=line " /* " a[t] " " t " */";
	    else
		line=line " /* " t " */";
	}
	else
	    line=line "NULL, /* " t " */";

	print line;

	show=0;
    }

    print "    (void *)-1L";
    print "};";
}

