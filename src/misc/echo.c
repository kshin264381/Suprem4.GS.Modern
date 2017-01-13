/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   echo.c                Version 5.1     */
/*   Last Modification : 7/3/91  08:41:07 */

#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "global.h"
#include "expr.h"
#include "misc.h"
#include "shell.h"
#include "check.h"

/************************************************************************
 *									*
 *	echo( par, param ) - this routine prints its parameter string.  *
 *									*
 ************************************************************************/
void echo(char *par, struct par_str* param)
{
    char tmp[512];
    char *err;
    struct vec_str *out;
    float val;

    if ( par == NULL ) {
	fprintf(stdout, "\n");
	fflush(stdout);
	return;
    }

    /*strip the leading spaces*/
    while( isspace ( *par ) ) par++;

    /*check for a real number expression*/
    strcpy(tmp, par);

    if ( (err = parse_expr( par, &out )) != NULL )  {
	fprintf(stdout, "%s\n", tmp);
	fprintf(stdout, "%s\n", err);
    }
    else if ( (err = eval_real( out, &val )) != NULL )  {
	fprintf(stdout, "%s\n", tmp);
	fprintf(stdout, "%s\n", err);
    }
    else 
	fprintf(stdout, "%g\n", val);
	
    fflush(stdout);
    if ( out != NULL ) free_expr( out );

}

/*-----------------uPause-----------------------------------------------
 * Wait before proceeding
 * Nice for reading someone else's input deck and checking it out on
 * the fly.
 *----------------------------------------------------------------------*/
void uPause(char *par, struct par_str* param)
{
    char *CommandBuf;
    char *s;

    if ( isatty( fileno(stdin) ) )  {
	for(;;) {
	    printf("Type <RETURN> to continue, or a command to be executed: ");
	    fflush(stdout);

#ifdef HAVE_KSH
	    s = read_line(NULL);
	    CommandBuf = salloc( char, strlen(s)+2 );
	    if (strlen(s) == 0) break;
	    strcpy(CommandBuf, s);
	    strcat(CommandBuf, "\n");
#else
	    CommandBuf = salloc( char, 256 );
	    //gets( CommandBuf); // --> Dangerous!! changed with fgets
	    fgets(CommandBuf, sizeof(CommandBuf), stdin);
	    if (!CommandBuf[0]) break;
#endif
	
	    do_string( CommandBuf, NULL, 0);
	    fflush( stdout);
	    fflush( stderr);
	}
    }
}



/************************************************************************
 *									*
 *	user_print( par, param ) - this routine prints its parameter string.  *
 *									*
 ************************************************************************/
void user_print(char *par, struct par_str* param)
{
    char *argv[50];
    char tmp[512];
    struct vec_str *out;
    float val;
    int i;

    if ( par == NULL ) {
	fprintf(stdout, "\n");
	fflush(stdout);
	return;
    }

    if ( par != NULL ) {
	if (split(par, argv, FALSE) == -1)
	    return;
    }

    for(i = 0; argv[i] != NULL;  i++) {

	/*check for a real number expression*/
	strcpy(tmp, argv[i]);

	if ( (parse_expr( tmp, &out )) != NULL ) 
	    fprintf(stdout, "%s ", argv[i]);
	else if ( (eval_real( out, &val )) != NULL ) 
	    fprintf(stdout, "%s ", argv[i]);
	else 
	    fprintf(stdout, "%g ", val);

	if ( out != NULL ) free_expr( out );
	free((char *) argv[i] );
    }
    fprintf(stdout, "\n" );
    return;
}
