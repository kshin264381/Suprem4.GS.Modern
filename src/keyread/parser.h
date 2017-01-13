/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CARD = 258,
    BG_GRP = 259,
    END_GRP = 260,
    PARTYPE = 261,
    EOL = 262,
    BOOLSTART = 263,
    GRT = 264,
    LES = 265,
    GRTE = 266,
    LESE = 267,
    OR = 268,
    AND = 269,
    EQU = 270,
    NOTEQU = 271,
    NOT = 272,
    PLUS = 273,
    TIMES = 274,
    RCONST = 275,
    STRING = 276,
    INTEGER = 277,
    NUMB = 278,
    NAME = 279,
    UNITS = 280,
    ERR_MSG = 281,
    COMMENT = 282,
    PARNAME = 283
  };
#endif
/* Tokens.  */
#define CARD 258
#define BG_GRP 259
#define END_GRP 260
#define PARTYPE 261
#define EOL 262
#define BOOLSTART 263
#define GRT 264
#define LES 265
#define GRTE 266
#define LESE 267
#define OR 268
#define AND 269
#define EQU 270
#define NOTEQU 271
#define NOT 272
#define PLUS 273
#define TIMES 274
#define RCONST 275
#define STRING 276
#define INTEGER 277
#define NUMB 278
#define NAME 279
#define UNITS 280
#define ERR_MSG 281
#define COMMENT 282
#define PARNAME 283

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 34 "parser.y" /* yacc.c:1909  */

    int ival;
    float dval;
    char *sval;
    struct bool_exp *bval;
    

#line 118 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
