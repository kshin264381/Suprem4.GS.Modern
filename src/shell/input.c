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
/*   input.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:28:08 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>

#include "constant.h"
#include "shell.h"
#include "plot.h"


/************************************************************************
 *									*
 *	get_input(in) - this reads a line of input from in and loads    *
 *  the input buffer with it.						*
 *									*
 *	Original	Mark E. Law		Sept, 1984		*
 *									*
 ************************************************************************/
void get_input(FILE *in, int per)
{
    //int i;
    size_t input_len;
    char *input_str;
    char *s;

    /*if nothing left in the super buffer*/
    if ( ! *(supbuf + supbpt) ) {

    /*clear the current pointers*/
    supbpt = 0;

    /*read input until and end of file or end of line*/
    if ( isatty( fileno(in) ) )  {
        s = read_line(prompt);
        strcpy(supbuf, s);
        strcat(supbuf, "\n");
        //i = strlen(supbuf);
        input_len = strlen(supbuf);
        free((char *)s);
    }
    else {
        //i = (int)fgets(supbuf, supbln, in);
        input_str = fgets(supbuf, supbln, in);
        // safeguard for strlen. If input_str ends up as NULL...
        // Not sure why but it happened...
        if (input_str) {
            input_len = strlen(input_str);
        }
        else {
            input_len = 0;
        }
    }

    /*fix up the values*/
    //if ( i == 0 ) {
    if (input_len == 0) {
        /*add on the newline or eof*/
        supbuf[0] = '\001';
        supbuf[1] = '\0';
    }
	check_x();
    }

    /*read an input line into the lex buffer and macro expand*/
    for( bufptr = 0;
         supbuf[supbpt] && (buffer[bufptr] = supbuf[supbpt]) != '\n';
         bufptr++, supbpt++);
    if ( supbuf[supbpt] == '\n' ) {
        buffer[bufptr++] = '\n';
        supbpt++;
    }
    buffer[bufptr++] = '\0';
    bufptr = 0;

    /*only macro expand a new line if we are reading and executing*/
    if ( (depth == -1) && per ) {
        /*do the macro expansions*/
        if (expand_macro( &buffer, &buflen, macro ) == -1) {
            strcpy(buffer, "\n");
        }
    }
}
   

#ifndef HAVE_KSH
char *read_line(char *prompt)
{
    char *s = (char *) malloc(128);

    fputs(prompt, stderr);
    (void)fgets(s, 128, stdin);
    return(s);
}
#endif
