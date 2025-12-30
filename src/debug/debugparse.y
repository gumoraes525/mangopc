%pure-parser
%parse-param {struct eval_command *resultptr}

%{

#define YYPARSE_PARAM resultptr

#include "debug/debugtype.h"
#include "debug/parsehelper.h"

void yyerror(struct eval_command *resultptr, char *s)
{
    set_eval_error(s);
}

%}

/* ============================
 * Semantic value types
 * ============================ */
%union {
    struct eval_command     command;
    struct eval_scalar      scalar;
    eval_commandtype        commandtoken;
    char                   *ident;
    struct eval_scalarlist  scalars;
}

/* ============================
 * Tokens
 * ============================ */

/* Scalars */
%token <scalar> EVAL_INT
%token <scalar> EVAL_FLOAT
%token <scalar> EVAL_STR

/* Registers (M68K) */
%token <scalar> EVAL_DREG
%token <scalar> EVAL_AREG
%token <scalar> EVAL_REG_PC
%token <scalar> EVAL_REG_SR

/* Identifiers */
%token <ident>  EVAL_IDENT

/* Commands */
%token <commandtoken> EVAL_PRINT
%token <commandtoken> EVAL_SETREG
%token <commandtoken> EVAL_REGS
%token <commandtoken> EVAL_BREAK
%token <commandtoken> EVAL_LIST_BREAK
%token <commandtoken> EVAL_STEP
%token <commandtoken> EVAL_NEXT
%token <commandtoken> EVAL_CONTINUE
%token <commandtoken> EVAL_QUIT
%token <commandtoken> EVAL_INSPECT_BYTE
%token <commandtoken> EVAL_INSPECT_HALF
%token <commandtoken> EVAL_INSPECT_WORD
%token <commandtoken> EVAL_INSPECT_DWORD
%token <commandtoken> EVAL_INSPECT_STRING
%token <commandtoken> EVAL_INSPECT_MEM
%token <commandtoken> EVAL_WATCH
%token <commandtoken> EVAL_WATCH_BYTE
%token <commandtoken> EVAL_WATCH_HALF
%token <commandtoken> EVAL_WATCH_WORD
%token <commandtoken> EVAL_WATCH_DWORD
%token <commandtoken> EVAL_DELETE_WATCH
%token <commandtoken> EVAL_DUMP
%token <commandtoken> EVAL_DISASM
%token <commandtoken> EVAL_HELP

/* Operators */
%token EVAL_POW
%token EVAL_SHL
%token EVAL_SHR
%token EVAL_EQ
%token EVAL_NE
%token EVAL_LT
%token EVAL_LE
%token EVAL_GT
%token EVAL_GE
%token EVAL_LAND
%token EVAL_LOR
%token EVAL_LXOR

/* ============================
 * Types
 * ============================ */
%type <command> command
%type <scalar>  scalar
%type <scalar>  reg
%type <scalar>  optional_scalar1
%type <scalar>  optional_scalar2
%type <scalars> scalarlist
%type <scalars> scalarlist_or_null
%type <ident>   identifier

/* ============================
 * Operator precedence
 * ============================ */
%left ','
%right '?' ':'
%left '.'
%left EVAL_LAND
%left EVAL_LXOR
%left EVAL_LOR
%left '|'
%left '^'
%left '&'
%nonassoc EVAL_EQ EVAL_NE EVAL_LT EVAL_LE EVAL_GT EVAL_GE
%nonassoc EVAL_SHL EVAL_SHR
%left '-' '+'
%left '*' '/' '%'
%left NEG
%left EVAL_POW

%%

/* ============================
 * Grammar
 * ============================ */

input:
      '\n'
        { create_command(resultptr, COMMAND_NOP, 0); }
    | command '\n'
        { *resultptr = $1; }
    | scalar '\n'
        { create_command(resultptr, COMMAND_PRINT, 1, &$1); }
;

command:
      EVAL_PRINT scalar
        { create_command(&$$, COMMAND_PRINT, 1, &$2); }
    | EVAL_REGS
        { create_command(&$$, COMMAND_REGS, 0); }
    | EVAL_SETREG reg '=' scalar
        { create_command(&$$, COMMAND_SETREG, 2, &$2, &$4); }
    | EVAL_BREAK scalar optional_scalar2
        { create_command(&$$, COMMAND_BREAK, 2, &$2, &$3); }
    | EVAL_LIST_BREAK
        { create_command(&$$, COMMAND_LIST_BREAK, 0); }
    | EVAL_STEP optional_scalar1
        { create_command(&$$, COMMAND_STEP, 1, &$2); }
    | EVAL_NEXT optional_scalar1
        { create_command(&$$, COMMAND_NEXT, 1, &$2); }
    | EVAL_CONTINUE
        { create_command(&$$, COMMAND_CONTINUE, 0); }
    | EVAL_QUIT
        { create_command(&$$, COMMAND_QUIT, 0); }
    | EVAL_INSPECT_BYTE scalar
        { create_command(&$$, COMMAND_INSPECT_BYTE, 1, &$2); }
    | EVAL_INSPECT_HALF scalar
        { create_command(&$$, COMMAND_INSPECT_HALF, 1, &$2); }
    | EVAL_INSPECT_WORD scalar
        { create_command(&$$, COMMAND_INSPECT_WORD, 1, &$2); }
    | EVAL_INSPECT_DWORD scalar
        { create_command(&$$, COMMAND_INSPECT_DWORD, 1, &$2); }
    | EVAL_INSPECT_STRING scalar
        { create_command(&$$, COMMAND_INSPECT_STRING, 1, &$2); }
    | EVAL_INSPECT_MEM scalar optional_scalar2
        { create_command(&$$, COMMAND_INSPECT_MEM, 2, &$2, &$3); }
    | EVAL_WATCH scalar optional_scalar2
        { create_command(&$$, COMMAND_WATCH, 2, &$2, &$3); }
    | EVAL_WATCH_BYTE scalar optional_scalar2
        { create_command(&$$, COMMAND_WATCH_BYTE, 2, &$2, &$3); }
    | EVAL_WATCH_HALF scalar optional_scalar2
        { create_command(&$$, COMMAND_WATCH_HALF, 2, &$2, &$3); }
    | EVAL_WATCH_WORD scalar optional_scalar2
        { create_command(&$$, COMMAND_WATCH_WORD, 2, &$2, &$3); }
    | EVAL_WATCH_DWORD scalar optional_scalar2
        { create_command(&$$, COMMAND_WATCH_DWORD, 2, &$2, &$3); }
    | EVAL_DELETE_WATCH scalar
        { create_command(&$$, COMMAND_DELETE_WATCH, 1, &$2); }
    | EVAL_DUMP scalar optional_scalar2
        { create_command(&$$, COMMAND_DUMP, 2, &$2, &$3); }
    | EVAL_DISASM optional_scalar1 optional_scalar2
        { create_command(&$$, COMMAND_DISASM, 2, &$2, &$3); }
    | EVAL_HELP
        { create_command(&$$, COMMAND_HELP, 0); }
;

/* ============================
 * Optional parameters
 * ============================ */

optional_scalar1:
      /* empty */
        { $$.type = SCALAR_EMPTY; }
    | scalar
        { $$ = $1; }
;

optional_scalar2:
      /* empty */
        { $$.type = SCALAR_EMPTY; }
    | ',' scalar
        { $$ = $2; }
;

/* ============================
 * Registers
 * ============================ */

reg:
      EVAL_DREG
    | EVAL_AREG
    | EVAL_REG_PC
    | EVAL_REG_SR
;

/* ============================
 * Scalars / expressions
 * ============================ */

scalar:
      EVAL_INT
    | EVAL_FLOAT
    | EVAL_STR
    | reg
    | scalar '*' scalar        { create_func_call(&$$, "*",  2, &$1, &$3); }
    | scalar '/' scalar        { create_func_call(&$$, "/",  2, &$1, &$3); }
    | scalar '%' scalar        { create_func_call(&$$, "%",  2, &$1, &$3); }
    | scalar '+' scalar        { create_func_call(&$$, "+",  2, &$1, &$3); }
    | scalar '-' scalar        { create_func_call(&$$, "-",  2, &$1, &$3); }
    | scalar '&' scalar        { create_func_call(&$$, "&",  2, &$1, &$3); }
    | scalar '|' scalar        { create_func_call(&$$, "|",  2, &$1, &$3); }
    | scalar '^' scalar        { create_func_call(&$$, "^",  2, &$1, &$3); }
    | scalar EVAL_POW scalar   { create_func_call(&$$, "**", 2, &$1, &$3); }
    | scalar EVAL_SHL scalar   { create_func_call(&$$, "<<", 2, &$1, &$3); }
    | scalar EVAL_SHR scalar   { create_func_call(&$$, ">>", 2, &$1, &$3); }
    | scalar EVAL_EQ scalar    { create_func_call(&$$, "==", 2, &$1, &$3); }
    | scalar EVAL_NE scalar    { create_func_call(&$$, "!=", 2, &$1, &$3); }
    | scalar EVAL_GT scalar    { create_func_call(&$$, ">",  2, &$1, &$3); }
    | scalar EVAL_GE scalar    { create_func_call(&$$, ">=", 2, &$1, &$3); }
    | scalar EVAL_LT scalar    { create_func_call(&$$, "<",  2, &$1, &$3); }
    | scalar EVAL_LE scalar    { create_func_call(&$$, "<=", 2, &$1, &$3); }
    | scalar EVAL_LAND scalar  { create_func_call(&$$, "&&", 2, &$1, &$3); }
    | scalar EVAL_LXOR scalar  { create_func_call(&$$, "^^", 2, &$1, &$3); }
    | scalar EVAL_LOR scalar   { create_func_call(&$$, "||", 2, &$1, &$3); }
    | '-' scalar %prec NEG     { create_func_call(&$$, "-",  1, &$2); }
    | '(' scalar ')'           { $$ = $2; }
    | scalar '?' scalar ':' scalar
        { create_func_call(&$$, "?:", 3, &$1, &$3, &$5); }
    | scalar '.' scalar
        { create_func_call(&$$, ".", 2, &$1, &$3); }
    | identifier '(' scalarlist_or_null ')'
        { create_func_call_list(&$$, $1, &$3); }
;

/* ============================
 * Lists
 * ============================ */

scalarlist_or_null:
      /* empty */
        { $$.count = 0; $$.scalars = NULL; }
    | scalarlist
;

scalarlist:
      scalar
        { scalarlist_set(&$$, &$1); }
    | scalarlist ',' scalarlist
        {
            scalarlist_concat(&$$, &$1, &$3);
            scalarlist_destroy_flat(&$1);
            scalarlist_destroy_flat(&$3);
        }
;

identifier:
      EVAL_IDENT
;

%%
