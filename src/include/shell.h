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
/*   shell.h                Version 5.1     */
/*   Last Modification : 7/3/91  08:55:21 */

#ifndef ShellH
#define ShellH

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN char *prompt;

/*these variables are used for input processing*/
EXTERN char *buffer;	/*the input buffer pointer*/
EXTERN int bufptr;	/*the current input buffer pointer*/
EXTERN int buflen;	/*the current amount of space buffer is decalared to*/

/*these are used in input munging as well*/
EXTERN char *supbuf;	/*the input buffer pointer*/
EXTERN int supbpt;	/*the current input buffer pointer*/
EXTERN int supbln;	/*the current amount of space buffer is decalared to*/

EXTERN char *echo_buffer;	/*the input buffer pointer*/
EXTERN int echo_bufptr;		/*the current input buffer pointer*/
EXTERN int echo_buflen;		/*the current amount of space buffer is decalared to*/

/*return values for prompting*/
#define BACKGROUND 1
#define PROMPT 2


/*define types of parameters that may be recieved*/
#define IVAL 1
#define DVAL 2
#define SVAL 3

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/*declare a function to parse a string and return a stream*/
extern FILE *file_parse();

/*this file is the standard input path for all input*/
EXTERN FILE *in_file ;

/*this structure is for use in the macro preprocessor*/
struct macro_table {
    char *name;		/*the name of the macro*/
    char *args;		/*the argument list if any for this macro*/
    char *replace;	/*the replacement string*/
    struct macro_table *next;	/*line pointer*/
};

EXTERN struct macro_table *macro;

/*set some performance vairables*/
EXTERN int noexecute;
EXTERN int interactive;
EXTERN int echoall;

/*this set of data is used in looping*/
/*store is a pointer to a pointer for storage mode on input*/
/*all data will be stored into this pointer*/
EXTERN char *store[10];
EXTERN int store_len[10];
EXTERN int store_ptr[10];
EXTERN int depth;

#include "data_types.h"

/*this structure and information is used to keep pointers to functions*/
struct command_table {
    char *name;
    void (*func)(char *parm, struct par_str* param);
    struct par_str* param;
    int cmd_num;
};

#define NUMCMD 50
extern struct command_table command[NUMCMD];

/*this table is used only with the fortran version*/
struct fort_str {
    int keyid;
    char *par;
    int back;
    char *rdir;
    int intr;
    struct fort_str *next;
};

/*pointers to the top and bottom of the command stack*/
EXTERN struct fort_str *fort_top, *fort_end;

/*file pointer for cpu file statistics*/
EXTERN FILE *cpufile;

void parser_boot( char *Pkeyfile, char *Pprompt);

//#ifdef __cplusplus
//extern "C"
//#endif
//int yyparse(void);

#ifdef __cplusplus
extern "C" {
#endif
#include "../shell/parser.h"
#ifdef __cplusplus
}
#endif

float string_to_real( char *expr, float defval);

#ifdef __cplusplus
extern "C"
#endif
void do_source(
char *file,	/*the file to be sourced*/
char *redir,	/*the filename for redirection, if any*/
int back,	/*flag for backgorund execution*/
int reperr)	/*report errors in the file open?*/;


#ifdef __cplusplus
extern "C"
#endif
void do_string(
char *instr,	/*the file to be sourced*/
char *redir,	/*the background file name if any*/
int back)	/*flag for backgorund execution*/;

void do_str(char *instr);

#ifdef __cplusplus
extern "C"
#endif
void do_command(
char *name,	/*the name of the command*/
char *param,	/*the command parameters*/
int intr,	/*the interactive flag*/
char *file,	/*the filename for redirect, NULL if no redirect*/
int back)	/*the flag for background of the command*/;

void do_exec(
char *par,
int intr,
int index,
int no_exec);

#ifdef __cplusplus
extern "C"
#endif
void help(char *par);

void print_help(char *space, struct par_str **param);

void get_input(
FILE *in,
int per);

char *read_line(
char *prompt);

char fetchc(FILE *in, int per);

#ifdef __cplusplus
extern "C"
#endif
int yylex();

int lex_command();

char* read_until(
char *str);

char* read_quote(char *str);

#ifdef __cplusplus
extern "C"
#endif
void do_echo();

#ifdef __cplusplus
extern "C"
#endif
char *list_parse(char **s, int first);

void loop_check(
char **str);

#ifdef __cplusplus
extern "C"
#endif
void dump_macro(struct macro_table *macro);

#ifdef __cplusplus
extern "C"
#endif
void define_macro(
char *str,
struct macro_table **macro);

int expand_macro(
char **expand_str,
int *len,
struct macro_table *macro);

#ifdef __cplusplus
extern "C"
#endif
int  undef_macro(
char *name,
struct macro_table **macro);

char* arg_expand(
char *s,
struct macro_table *mac);

void parser_boot(
     char *Pkeyfile,
     char *Pprompt);

int get_proc(char *name);

int substring(
    char *s, char *ss);

#ifdef __cplusplus
extern "C"
#endif
FILE *file_parse(
char *name,
char *type);

#ifdef __cplusplus
extern "C"
#endif
void do_set(
char *par,
int on);

float string_to_real(
     char *expr,
     float defval);

void smacro(
     char *name, char *def);

void fmacro(
     char *name, char *format,
     float val);

void umacro(
     char *name);

#endif
