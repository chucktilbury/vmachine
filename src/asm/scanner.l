
%{
//#undef //_DEBUGGING
//#undef //_TRACE
//#define //_TRACE(...)

#include "common.h"
//#include "vm_support.h"
#include "parser.h"
#include "scanner.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

// maximum depth of imports prevents crashing from circular imports.
//static int import_depth = 0;
//#define MAX_IMPORTS 15

// string buffer to get token from
static char strbuf[1024*64];

typedef struct _file_name_stack {
    int line_no;
    int col_no;
    char *name;
    struct _file_name_stack *next;
} _file_name_stack;

void count(void);
static char buffer[1024*64];
static int bidx = 0;
static _file_name_stack *name_stack;
int num_errors = 0; // global updated by parser

// these funcs support the string scanner
static void __append_char(char ch) {
    if((sizeof(buffer)-1) > (size_t)bidx) {
        buffer[bidx] = ch;
        bidx++;
    }
    else {
        fatal_error("buffer overrun");
        //fprintf(stderr, "scanner buffer overrun");
        exit(1);
    }
}

static void __append_str(char *str) {
    if((sizeof(buffer)-1) > (strlen(buffer) + strlen(str))) {
        strcat(buffer, str);
        bidx = strlen(buffer);
    }
    else {
        fatal_error("buffer overrun");
        //fprintf(stderr, "scanner buffer overrun");
        exit(1);
    }
}

static void update_loc(void){

    if(NULL != name_stack) {
        yylloc.first_line   = name_stack->line_no;
        yylloc.first_column = name_stack->col_no;
    }
    else
        return;

    for(char *s = yytext; *s != '\0'; s++) {
        name_stack->col_no++;
    }

    yylloc.last_line   = name_stack->line_no;
    yylloc.last_column = name_stack->col_no-1;
}

#define YY_USER_ACTION update_loc();

%}

/* state definitions */
%x SQUOTES
%x DQUOTES
%x COMMENT
%option noinput nounput
%option yylineno
%option noyywrap
%%

    /* overhead */
\n              { name_stack->line_no++; name_stack->col_no=1; } // yylineno++; }
[ \t\r]         {}

    /* keywords */
"string"        { yylval.type = VAL_STRING; return TOK_STR_TYPE; }
"float"         { yylval.type = VAL_FNUM; return TOK_FNUM_TYPE; }
"int"           { yylval.type = VAL_INUM; return TOK_INUM_TYPE; }
"uint"          { yylval.type = VAL_UNUM; return TOK_UNUM_TYPE; }
"bool"          { yylval.type = VAL_BOOL; return TOK_BOOL_TYPE; }

"const"         { return TOK_CONST; }
"true"          { yylval.type = TOK_TRUE; return TOK_TRUE; }
"false"         { yylval.type = TOK_FALSE; return TOK_FALSE; }

"cast"          { yylval.opcode = OP_CAST; return TOK_CAST; }
"error"         { yylval.opcode = OP_ERROR; return TOK_ERROR; }
"noop"          { yylval.opcode = OP_NOOP; return TOK_NOOP; }
"exit"          { yylval.opcode = OP_EXIT; return TOK_EXIT; }
"call"          { yylval.opcode = OP_CALL; return TOK_CALL; }
"callx"         { yylval.opcode = OP_CALLX; return TOK_CALLX; }
"return"        { yylval.opcode = OP_RETURN; return TOK_RETURN; }
"ret"           { yylval.opcode = OP_RETURN; return TOK_RETURN; }
"jmp"           { yylval.opcode = OP_JMP; return TOK_JMP; }
"jmpif"         { yylval.opcode = OP_JMPIF; return TOK_JMPIF; }
"except"        { yylval.opcode = OP_EXCEPT; return TOK_EXCEPT; }
"push"          { yylval.opcode = OP_PUSH; return TOK_PUSH; }
"save"          { yylval.opcode = OP_SAVE; return TOK_SAVE; }
"pop"           { yylval.opcode = OP_POP; return TOK_POP; }
"not"           { yylval.opcode = OP_NOT; return TOK_NOT; }
"eq"            { yylval.opcode = OP_EQ; return TOK_EQ; }
"neq"           { yylval.opcode = OP_NEQ; return TOK_NEQ; }
"leq"           { yylval.opcode = OP_LEQ; return TOK_LEQ; }
"geq"           { yylval.opcode = OP_GEQ; return TOK_GEQ; }
"less"          { yylval.opcode = OP_LESS; return TOK_LESS; }
"gtr"           { yylval.opcode = OP_GTR; return TOK_GTR; }
"neg"           { yylval.opcode = OP_NEG; return TOK_NEG; }
"add"           { yylval.opcode = OP_ADD; return TOK_ADD; }
"sub"           { yylval.opcode = OP_SUB; return TOK_SUB; }
"mul"           { yylval.opcode = OP_MUL; return TOK_MUL; }
"div"           { yylval.opcode = OP_DIV; return TOK_DIV; }
"mod"           { yylval.opcode = OP_MOD; return TOK_MOD; }
"prints"        { yylval.opcode = OP_PRINTS; return TOK_PRINTS; }
"print"         { yylval.opcode = OP_PRINT; return TOK_PRINT; }
"peek"          { yylval.opcode = OP_PEEK; return TOK_PEEK; }
"local"         { yylval.opcode = OP_LOCAL; return TOK_LOCAL; }

        /* arithmetic operators */
"+"             { return '+'; }
"-"             { return '-'; }
"="             { return '='; }
"/"             { return '/'; }
"*"             { return '*'; }
"%"             { return '%'; }
"("             { return '('; }
")"             { return ')'; }

    /* constructed tokens */

    /* A simple symbol */
[a-zA-Z_][a-zA-Z_0-9]* {
        yylval.str = (char*)strdup(yytext);
        //printf("SYMBOL: %s\n", yytext);
        return TOK_SYMBOL;
    }

    /* recognize and ignore comments */
[/][*]+ { BEGIN(COMMENT); }
<COMMENT>[*]+[/] { BEGIN(INITIAL); }
<COMMENT>\n { name_stack->line_no++; yylineno++; }
<COMMENT>.  {}  /* eat everything in between */

    /* eat up until the newline */
[/][/].* { ;
    }

[0-9]+ {
        //yylval.literal.intval = strtol(yytext, NULL, 10);
        yylval.inum = (int32_t)strtol(yytext, NULL, 10);
        return TOK_INUM;
    }

    /* recognize a float */
([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)? {
        //yylval.literal.fpval = strtod(yytext, NULL);
        yylval.fnum = strtof(yytext, NULL);
        //printf("number: %0.3f\n", yylval.fnum);
        return TOK_FNUM;
    }

0[Xx][0-9a-fA-F]+ {
        //yylval.literal.uintval = strtol(yytext, NULL, 16);
        yylval.unum = (uint32_t)strtol(yytext, NULL, 16);
        return TOK_UNUM;
    }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" {
        //yylval.literal.str = STRDUP(buffer);
        yylval.str = (char*)strdup(buffer);
        BEGIN(INITIAL);
        return TOK_STR;
    }

    /* the short rule matches before the long one does */
<DQUOTES>\\n    { __append_char('\n'); }
<DQUOTES>\\r    { __append_char('\r'); }
<DQUOTES>\\e    { __append_char('\x1b'); }
<DQUOTES>\\t    { __append_char('\t'); }
<DQUOTES>\\b    { __append_char('\b'); }
<DQUOTES>\\f    { __append_char('\f'); }
<DQUOTES>\\v    { __append_char('\v'); }
<DQUOTES>\\\\   { __append_char('\\'); }
<DQUOTES>\\\"   { __append_char('\"'); }
<DQUOTES>\\\'   { __append_char('\''); }
<DQUOTES>\\\?   { __append_char('\?'); }
<DQUOTES>\\.    { __append_char(yytext[1]); }
<DQUOTES>\\[0-7]{1,3} { __append_char((char)strtol(yytext+1, 0, 8));  }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { __append_char((char)strtol(yytext+2, 0, 16));  }
<DQUOTES>[^\\\"\n]*  { __append_str(yytext); }
<DQUOTES>\n     { name_stack->line_no++; yylineno++; } /* track line numbers, but strip new line */


    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' {
        //yylval.literal.str = STRDUP(buffer);
        yylval.str = (char*)strdup(buffer);
        BEGIN(INITIAL);
        return TOK_STR;
    }

<SQUOTES>[^\\'\n]*  { __append_str(yytext); }
<SQUOTES>\\.    { __append_str(yytext); }
<SQUOTES>\n     { __append_str(yytext); name_stack->line_no++; yylineno++; } /* don't strip new lines */

.               { printf("Warning: unrecognized character: %c (0x%02X)", yytext[0], yytext[0]);}

<<EOF>> {

        _file_name_stack *name = name_stack->next;
        _free(name_stack->name);
        _free(name_stack);
        name_stack = name;

        memset(strbuf, 0, sizeof(strbuf));
        yypop_buffer_state();
        if(!YY_CURRENT_BUFFER) {
            yyterminate();
        }
        //import_depth--;

        //return EOF;
    }

%%

int open_file(const char *fname) {

    _file_name_stack *name;

    name = _alloc_ds(_file_name_stack); //calloc(1, sizeof(_file_name_stack));
    name->name = (char*)_strdup(fname);
    name->next = name_stack;
    name->line_no = 1;
    name->col_no = 1;
    name_stack = name;

    yyin = fopen(fname, "r");
    if(NULL == yyin) {
        fatal_error("cannot open the input file: \"%s\": %s", fname, strerror(errno));
        return 1;
    }

    yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
    return 0;
}

// Tracking and global interface
char *get_file_name(void) {
    if(NULL != name_stack)
        return name_stack->name;
    else
        return "no name";
}

int get_line_number(void) {
    if(NULL != name_stack)
        return name_stack->line_no;
    else
        return -1;
}

int get_col_number(void) {
    if(NULL != name_stack)
        return name_stack->col_no;
    else
        return -1;
}

const char *get_tok_str(void) {
    return strbuf;
}
