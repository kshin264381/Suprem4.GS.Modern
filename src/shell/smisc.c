/*----------------------------------------------------------------------
 *
 * smisc.c - easier interface to expression parser
 *
 * Copyright c 1985 The board of trustees of the Leland Stanford
 *                  Junior University. All rights reserved.
 * This subroutine may not be used outside of the SUPREM4 computer
 * program without the prior written consent of Stanford University.
 *
 * Original: CSR Thu Jul 16 16:47:28 1987
 *---------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>

#include "global.h"
#include "expr.h"
#include "check.h"
#include "shell.h"
#include "misc.h"


/*-----------------STRING_TO_REAL---------------------------------------
 * Hide everything from the innocent users
 *----------------------------------------------------------------------*/
float string_to_real(
     char *expr,
     float defval)
{
    char *err, *dexpr;
    struct vec_str *rexp; int len;
    float val;

    /* First make a copy that we can write all over */
    len = 1 + strlen( expr);
    dexpr = salloc(char , len);
    strcpy( dexpr, expr);

    /* Expand macros */
    (void)expand_macro( &dexpr, &len, macro);

    /* Try to parse */
    if((err = parse_expr( dexpr, &rexp)) || (err = eval_real( rexp, &val))) {
	fprintf( stderr, "bad expression: %s\n", err);
	val = defval;
    }
    free_expr( rexp);
    sfree( dexpr);
    return( val);
}

/*-----------------SMACRO, FMACRO, UMACRO-------------------------------
 * Define and undefine macros without including the whole expression parser.
 *----------------------------------------------------------------------*/

void smacro(
     char *name, char *def)
{
    char buffer[512];
    sprintf( buffer, "%s %s", name, def);
    define_macro( buffer, &macro);
}

void fmacro(
     char *name, char *format,
     float val)
{
    char buffer[512], formbuf[512];
    strcpy( formbuf, "%s ");
    strcat( formbuf, format);
    sprintf( buffer, formbuf, name, val);
    define_macro( buffer, &macro);
}

void umacro(
     char *name)
{
    (void)undef_macro( name, &macro);
}
