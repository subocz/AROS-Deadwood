/*
    Copyright (C) 1995-2015, The AROS Development Team. All rights reserved.

    The code for storing information of functions present in the module
*/
#include <string.h>
#include <assert.h>
#include "functionhead.h"
#include "config.h"

/* getargtype remove the variable name from a variable definition and leave return
 * the type of the variable
 * [] at the end will be added as * in the variable type
 * e.g. char *var[] => type: char **, name: var
 * This is a destructive function and will change to string pointed to by def
 * to only contain the type afterwards.
 * Function return 0 when it did not understand the input, 1 otherwise
 */
static char *getargtype(struct functionarg *funcarg);
static char *getargname(const struct functionarg *funcarg);


struct functionhead *newfunctionhead(const char *name, enum libcall libcall)
{
    struct functionhead *funchead = malloc(sizeof(struct functionhead));

    if (funchead != NULL)
    {
        funchead->next = NULL;
        funchead->internalname = funchead->name = strdup(name);
        funchead->type = NULL;
        funchead->libcall = libcall;
        funchead->lvo = 0;
        funchead->argcount = 0;
        funchead->arguments = NULL;
        funchead->aliases = NULL;
        funchead->interface = NULL;
        funchead->method = NULL;
        funchead->varargtype = 0;
        funchead->novararg = 0;
        funchead->priv= 0;
        funchead->unusedlibbase = 0;
        funchead->deprecated = 0;
    }
    else
    {
        puts("Out of memory !");
        exit(20);
    }

    return funchead;
}

struct functionarg *funcaddarg
(
    struct functionhead *funchead,
    const char *arg, const char *reg
)
{
    struct functionarg **argptr = &funchead->arguments;

    while ((*argptr) != NULL) argptr = &(*argptr)->next;

    *argptr = malloc(sizeof(struct functionarg));
    if (*argptr != NULL)
    {
        (*argptr)->next = NULL;
        (*argptr)->arg  = (arg == NULL) ? NULL : strdup(arg);
        (*argptr)->type = NULL;
        (*argptr)->name = NULL;
        (*argptr)->reg  = (reg  == NULL) ? NULL : strdup(reg);
        (*argptr)->parent  = funchead;
        (*argptr)->ellipsis = 0;

        /* Set noargname, varargs, type, name here (in a better way) */
        free(getargtype((*argptr)));

        funchead->argcount++;
    }
    else
    {
        puts("Out of memory !");
        exit(20);
    }

    return *argptr;
}

struct stringlist *funcaddalias(struct functionhead *funchead, const char *alias)
{
    return slist_append(&funchead->aliases, alias);
}

void funcsetinternalname(struct functionhead *funchead, const char *internalname)
{
    if (funchead->name != funchead->internalname)
        free(funchead->internalname);
    funchead->internalname = strdup(internalname);
}

void writefuncdefs(FILE *out, struct config *cfg, struct functionhead *funclist)
{
    struct functionhead *funclistit;
    struct functionarg *arglistit;
    char *type, *name;
    int first;

    for(funclistit = funclist; funclistit != NULL; funclistit = funclistit->next)
    {
        switch (funclistit->libcall)
        {
        case STACK:
            fprintf(out, "%s %s(", funclistit->type, funclistit->internalname);

            for(arglistit = funclistit->arguments, first = 1;
                arglistit != NULL;
                arglistit = arglistit->next, first = 0
            )
            {
                if (!first)
                    fprintf(out, ", ");

                fprintf(out, "%s", arglistit->arg);
            }
            fprintf(out, ");\n");
            break;

        case REGISTER:
            assert(cfg);
            fprintf(out, "%s %s(", funclistit->type, funclistit->internalname);
            for (arglistit = funclistit->arguments, first = 1;
                 arglistit!=NULL;
                 arglistit = arglistit->next, first = 0
            )
            {
                if (!first)
                    fprintf(out, ", ");
                fprintf(out, "%s", arglistit->arg);
            }
            fprintf(out,
                    ");\nAROS_LH%d%s(%s, %s,\n",
                    funclistit->argcount, funclistit->unusedlibbase ? "I" : "",
                    funclistit->type, funclistit->internalname
            );
            for (arglistit = funclistit->arguments;
                 arglistit!=NULL;
                 arglistit = arglistit->next
            )
            {
                type = getargtype(arglistit);
                name = getargname(arglistit);
                assert(name != NULL && type != NULL);

                fprintf(out,
                        "         AROS_LHA(%s, %s, %s),\n",
                        type, name, arglistit->reg
                );
                free(type);
                free(name);
            }
            fprintf(out,
                    "         %s, %s, %u, %s)\n"
                    "{\n"
                    "    AROS_LIBFUNC_INIT\n\n"
                    "    %s%s(",
                    cfg->libbasetypeptrextern, cfg->libbase, funclistit->lvo, cfg->basename,
                    (strcasecmp(funclistit->type, "void") == 0) ? "" : "return ",
                    funclistit->internalname
            );
            for (arglistit = funclistit->arguments, first = 1;
                 arglistit!=NULL;
                 arglistit = arglistit->next, first = 0
            )
            {
                name = getargname(arglistit);
                assert(name != NULL);

                if (!first)
                    fprintf(out, ", ");
                fprintf(out, "%s", name);
                free(name);
            }
            fprintf(out,
                    ");\n\n"
                    "    AROS_LIBFUNC_EXIT\n"
                    "}\n\n");
            break;

        case REGISTERMACRO:
            assert(cfg);
            if (funclistit->arguments == NULL
                || strchr(funclistit->arguments->reg, '/') == NULL)
            {
                fprintf(out,
                        "AROS_LD%d%s(%s, %s,\n",
                        funclistit->argcount, funclistit->unusedlibbase ? "I" : "",
                        funclistit->type, funclistit->internalname
                );
                for (arglistit = funclistit->arguments;
                     arglistit!=NULL;
                     arglistit = arglistit->next
                )
                {
                    type = getargtype(arglistit);
                    name = getargname(arglistit);
                    assert(type != NULL && name != NULL);

                    fprintf(out,
                            "         AROS_LDA(%s, %s, %s),\n",
                            type, name, arglistit->reg
                    );
                    free(type);
                    free(name);
                }
                fprintf(out,
                        "         LIBBASETYPEPTR, %s, %u, %s\n"
                        ");\n",
                        cfg->libbase, funclistit->lvo, cfg->basename
                );
            }
            else
            {
                fprintf(out,
                        "AROS_LDQUAD%d(%s, %s,\n",
                        funclistit->argcount, funclistit->type, funclistit->internalname
                );
                for (arglistit = funclistit->arguments;
                     arglistit != NULL;
                     arglistit = arglistit->next
                )
                {
                    if (strlen(arglistit->reg) != 5)
                    {
                        fprintf(stderr, "Internal error: ../.. register format expected\n");
                        exit(20);
                    }
                    arglistit->reg[2] = 0;

                    type = getargtype(arglistit);
                    name = getargname(arglistit);
                    assert(type != NULL && name != NULL);

                    fprintf(out,
                            "         AROS_LDAQUAD(%s, %s, %s, %s),\n",
                            type, name, arglistit->reg, arglistit->reg+3
                    );
                    arglistit->reg[2] = '/';
                    free(type);
                    free(name);
                }
                fprintf(out,
                        "         LIBBASETYPEPTR, %s, %u, %s\n"
                        ");\n",
                        cfg->libbase, funclistit->lvo, cfg->basename
                );
            }
            break;

        default:
            fprintf(stderr, "Internal error: unhandled libcall in writefuncdefs\n");
            exit(20);
            break;
        }
    }
}

void writefuncprotos(FILE *out, struct config *cfg, struct functionhead *funclist)
{
    struct functionhead *funclistit;
    struct functionarg *arglistit;
    char *type, *name;
    int first;

    for(funclistit = funclist; funclistit != NULL; funclistit = funclistit->next)
    {
        if (!funclistit->priv && (funclistit->lvo >= cfg->firstlvo))
        {
            fprintf(out,
                    "\n"
                    "#if !defined(__%s_LIBAPI__) || (%d <= __%s_LIBAPI__)"
                    "\n",
                    cfg->includenameupper,
                    funclistit->version,
                    cfg->includenameupper
            );

            if (funclistit->deprecated) {
                fprintf(out,
                        "\n"
                        "#if defined(__AROS_GIMME_DEPRECATED__)"
                        "\n");
            }

            switch (funclistit->libcall)
            {
            case STACK:
                fprintf(
                    out,
                    "#ifndef %s\n"
                    "%s %s(",
                    funclistit->name,
                    funclistit->type, funclistit->name
                );

                for(arglistit = funclistit->arguments, first = 1;
                    arglistit != NULL;
                    arglistit = arglistit->next, first = 0
                )
                {
                    if (!first)
                        fprintf(out, ", ");

                    fprintf(out, "%s", arglistit->arg);
                }
                fprintf(out, ");\n#endif\n");
                break;

            case REGISTER:
            case REGISTERMACRO:
                assert(cfg);
                if (funclistit->arguments == NULL
                    || strchr(funclistit->arguments->reg, '/') == NULL
                )
                {
                    fprintf(out,
                            "AROS_LP%d%s(%s, %s,\n",
                            funclistit->argcount, funclistit->unusedlibbase ? "I" : "",
                            funclistit->type, funclistit->name
                    );
                    for (arglistit = funclistit->arguments;
                        arglistit!=NULL;
                        arglistit = arglistit->next
                    )
                    {
                        type = getargtype(arglistit);
                        name = getargname(arglistit);
                        assert(type != NULL && name != NULL);

                        fprintf(out,
                                "         AROS_LPA(%s, %s, %s),\n",
                                type, name, arglistit->reg
                        );
                        free(type);
                        free(name);
                    }
                    fprintf(out,
                            "         LIBBASETYPEPTR, %s, %u, %s\n"
                            ");\n",
                            cfg->libbase, funclistit->lvo, cfg->basename
                    );
                }
                else
                {
                    fprintf(out,
                            "AROS_LPQUAD%d(%s, %s,\n",
                            funclistit->argcount, funclistit->type, funclistit->name
                    );
                    for (arglistit = funclistit->arguments;
                        arglistit != NULL;
                        arglistit = arglistit->next
                    )
                    {
                        if (strlen(arglistit->reg) != 5)
                        {
                            fprintf(stderr, "Internal error: ../.. register format expected\n");
                            exit(20);
                        }
                        arglistit->reg[2] = 0;

                        type = getargtype(arglistit);
                        name = getargname(arglistit);
                        assert(type != NULL && name != NULL);

                        fprintf(out,
                                "         AROS_LPAQUAD(%s, %s, %s, %s),\n",
                                type, name, arglistit->reg, arglistit->reg+3
                        );
                        arglistit->reg[2] = '/';
                        free(type);
                        free(name);
                    }
                    fprintf(out,
                            "         LIBBASETYPEPTR, %s, %u, %s\n"
                            ");\n",
                            cfg->libbase, funclistit->lvo, cfg->basename
                    );
                }
                break;

            default:
                fprintf(stderr, "Internal error:"
                    " unhandled libcall in writefuncprotos\n");
                exit(20);
                break;
            }

            if (funclistit->deprecated) {
                fprintf(out,
                        "\n"
                        "#endif /* defined(__AROS_GIMME_DEPRECATED__) */"
                        "\n");
            }

            fprintf(out,
                    "\n"
                    "#endif /* !defined(__%s_LIBAPI__) || (%d <= __%s_LIBAPI__) */"
                    "\n",
                    cfg->includenameupper,
                    funclistit->version,
                    cfg->includenameupper
            );
        }
    }
}

void writefuncinternalstubs(FILE *out, struct config *cfg, struct functionhead *funclist)
{
    struct functionhead *funclistit;
    struct functionarg *arglistit;
    char *type, *name;
    int first;

    for(funclistit = funclist; funclistit != NULL; funclistit = funclistit->next)
    {
        switch (funclistit->libcall)
        {
        case STACK:
#if defined(LIBBASE_IN_REG)
            fprintf(out,
                    "AROS_GM_STACKALIAS(%s,%s,%d);\n"
                    , funclistit->internalname
                    , cfg->basename
                    , funclistit->lvo
            );
#else
            if ((cfg->options & OPTION_DUPBASE)
                && !funclistit->unusedlibbase
            )
            {
                fprintf(out,
                        "AROS_GM_STACKCALL(%s,%s,%d);\n"
                        , funclistit->internalname
                        , cfg->basename
                        , funclistit->lvo
                );
            }
            else
            {
                fprintf(out,
                        "AROS_GM_STACKALIAS(%s,%s,%d);\n"
                        , funclistit->internalname
                        , cfg->basename
                        , funclistit->lvo
                );
            }
#endif
            break;

        case REGISTER:
        case REGISTERMACRO:
            /* NOP */
            break;

        default:
            fprintf(stderr, "Internal error: "
                "unhandled libcall in writefuncinternalstubs\n");
            exit(20);
            break;
        }
    }
}

static char *getargtype(struct functionarg *funcarg)
{
    char *s, *begin, *end;
    unsigned int brackets = 0, i;

    begin = s = strdup(funcarg->arg);

    /* Count the [] at the end of the argument */
    end = begin+strlen(begin);
    while (isspace(*(end-1))) end--;

    while (*(end-1)==']')
    {
        brackets++;
        end--;
        while (isspace(*(end-1)) || isalnum(*(end-1))) end--;
        if (*(end-1)!='[')
        {
            free(s);
            return NULL;
        }
        end--;
        while (isspace(*(end-1))) end--;
    }

    /* Special case of pointer to function */
    if (*(end-1)==')')
    {
        char *pb = NULL, *pe = NULL;
        int len = 0;
        pb = begin;
        while (*(pb)!='(') pb++;
        while (*(pb)!='*') pb++;
        pb++;
        pe=pb;
        while (*(pe)!=')') pe++;
        len = pe-pb;
        funcarg->name = malloc(len+1);
        strncpy(funcarg->name, pb, len);
        funcarg->name[len] = '\0';

        while ((*pe)!='\0')
        {
            *pb=*pe;
            pb++;pe++;
        }
        *pb='\0';
        funcarg->type = strdup(s);

        return s;
    }
    else
    {
    /* Skip over the argument name */
    while (!isspace(*(end-1)) && *(end-1)!='*')
    {
        if (begin == end)
        {
            /* Support special cases */
            if ((strcmp(s, "void") == 0) || (strcmp(s, "...") == 0))
            {
                if (funcarg->type == NULL) funcarg->type = strdup(s);
                if (funcarg->name == NULL) funcarg->name = strdup("");
                if (strcmp(s, "...") == 0) funcarg->ellipsis = 1;
                return s;
            }
            else
            {
                free(s);
                fprintf(stderr, "no argument type or name found for arg: %s in function %s\n", funcarg->arg, funcarg->parent->name);
                exit(20);
            }
        }
        end--;
    }
    }

    /* Add * for the brackets */
    while (isspace(*(end-1))) end--;
    for (i=0; i<brackets; i++)
    {
        *end='*';
        end++;
    }
    *end='\0';

    if (funcarg->type == NULL) funcarg->type = strdup(s);
    if (funcarg->name == NULL) funcarg->name = getargname(funcarg);

    return s;
}

static char *getargname(const struct functionarg *funcarg)
{
    char *s, *begin, *end;
    int len;

    /* Count the [] at the end of the argument */
    end = funcarg->arg+strlen(funcarg->arg);
    while (isspace(*(end-1))) end--;
    while (*(end-1)==']')
    {
        end--;
        while (isspace(*(end-1)) || isalnum(*(end-1))) end--;
        if (*(end-1)!='[')
            return NULL;
        end--;
        while (isspace(*(end-1))) end--;
    }

    /* Go to the beginning of the argument name */
    begin = end;
    while (!isspace(*(begin-1)) && *(begin-1)!='*') begin--;

    /* Copy the name */
    len = end - begin;
    s = malloc(len+1);
    strncpy(s, begin, len);
    s[len] = '\0';

    return s;
}
