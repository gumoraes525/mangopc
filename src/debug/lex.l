%{
#define USE_PURE_PARSER

#ifdef USE_PURE_PARSER
#define YY_DECL int yylex (YYSTYPE *lvalp)
#define YY_LVALP lvalp
#else
#define YY_LVALP (&yylval)
#endif

/* get current pos (offset from start of line) */
#define YY_USER_ACTION lex_pos = yy_bp - YY_CURRENT_BUFFER->yy_ch_buf;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "debug/debugtype.h"
#include "debugparse.h"
#include "parsehelper.h"

static int lex_pos;

/* ---------- helpers ---------- */

static int char2int(char a, int base)
{
    int i;
    if (a >= 'A' && a <= 'Z') i = a - 'A' + 10;
    else if (a >= 'a' && a <= 'z') i = a - 'a' + 10;
    else if (a >= '0' && a <= '9') i = a - '0';
    else return -1;
    return (i >= base) ? -1 : i;
}

static int parse_integer(struct eval_scalar *i, char *num, int base, int lenmod)
{
    uint64 k = 0;
    int l = strlen(num) + lenmod;
    while (l--) {
        int c = char2int(*num++, base);
        if (c < 0) return 0;
        k = k * base + c;
    }
    i->type = SCALAR_INT;
    i->scalar.integer.value = k;
    i->scalar.integer.type = TYPE_UNKNOWN;
    return 1;
}

static int parse_float(struct eval_scalar *f, char *fpn)
{
    char *end;
    double d = strtod(fpn, &end);
    if (*end == 0) {
        f->type = SCALAR_FLOAT;
        f->scalar.floatnum.value = d;
        return 1;
    }
    return 0;
}

/* ---------- M68k register parsing ---------- */

static int parse_dreg(struct eval_scalar *i, char *s)
{
    if (tolower(s[0]) != 'd') return 0;
    if (s[1] < '0' || s[1] > '7' || s[2] != 0) return 0;
    i->type = SCALAR_REG;
    i->scalar.reg.type = REG_D;
    i->scalar.reg.num  = s[1] - '0';
    return 1;
}

static int parse_areg(struct eval_scalar *i, char *s)
{
    if (tolower(s[0]) != 'a') return 0;
    if (s[1] < '0' || s[1] > '7' || s[2] != 0) return 0;
    i->type = SCALAR_REG;
    i->scalar.reg.type = REG_A;
    i->scalar.reg.num  = s[1] - '0';
    return 1;
}

static int parse_cstring(struct eval_scalar *r, char *s, int len)
{
    char *out = (char*)malloc(len ? len : 1);
    char *dst = out;

    r->type = SCALAR_STR;
    r->scalar.str.value = out;

    while (len--) {
        if (*s == '\\' && len) {
            s++; len--;
            switch (*s) {
                case 'n': *dst++ = '\n'; break;
                case 'r': *dst++ = '\r'; break;
                case 't': *dst++ = '\t'; break;
                case '\\':*dst++ = '\\'; break;
                case '"': *dst++ = '"'; break;
                default:  *dst++ = *s; break;
            }
        } else {
            *dst++ = *s;
        }
        s++;
    }

    r->scalar.str.len = dst - out;
    return 1;
}

/* ---------- buffer helpers ---------- */

void *lex_current_buffer()        { return (void*)YY_CURRENT_BUFFER; }
int   lex_current_buffer_pos()    { return lex_pos; }
void  lex_switch_buffer(void *b)  { yy_switch_to_buffer(b); }
void  lex_delete_buffer(void *b)  { yy_delete_buffer(b); }
void *lex_scan_string_buffer(const char *s) { return yy_scan_string(s); }

%}

%option noyywrap

%%

[ \t]+                          /* skip */

\"(\\\"|[^"])*\"                if (parse_cstring(&YY_LVALP->scalar, yytext+1, strlen(yytext)-2)) return EVAL_STR;
'[^']*'                         if (parse_cstring(&YY_LVALP->scalar, yytext+1, strlen(yytext)-2)) return EVAL_STR;

/* operators */
\*\*            return EVAL_POW;
\<\<            return EVAL_SHL;
\>\>            return EVAL_SHR;
\<\=            return EVAL_LE;
\>\=            return EVAL_GE;
\<              return EVAL_LT;
\>              return EVAL_GT;
\=\=            return EVAL_EQ;
\!\=            return EVAL_NE;
\&\&            return EVAL_LAND;
\|\|            return EVAL_LOR;
\^\^            return EVAL_LXOR;

/* debugger commands */
print|p         return EVAL_PRINT;
regs            return EVAL_REGS;
setreg          return EVAL_SETREG;
break|b         return EVAL_BREAK;
step|s          return EVAL_STEP;
next|n          return EVAL_NEXT;
continue|c      return EVAL_CONTINUE;
quit|bye        return EVAL_QUIT;
dump|d          return EVAL_DUMP;
disasm|u        return EVAL_DISASM;
help|h          return EVAL_HELP;
watch|w         return EVAL_WATCH;
delete_watch|dw return EVAL_DELETE_WATCH;

/* M68k registers */
[pP][cC] {
    YY_LVALP->scalar.type = SCALAR_REG;
    YY_LVALP->scalar.scalar.reg.type = REG_PC;
    return EVAL_REG;
}

[sS][rR] {
    YY_LVALP->scalar.type = SCALAR_REG;
    YY_LVALP->scalar.scalar.reg.type = REG_SR;
    return EVAL_REG;
}

[sS][pP] {
    YY_LVALP->scalar.type = SCALAR_REG;
    YY_LVALP->scalar.scalar.reg.type = REG_A;
    YY_LVALP->scalar.scalar.reg.num  = 7;
    return EVAL_REG;
}

[dD][0-7]        if (parse_dreg(&YY_LVALP->scalar, yytext)) return EVAL_REG;
[aA][0-7]        if (parse_areg(&YY_LVALP->scalar, yytext)) return EVAL_REG;

/* identifiers */
[$@a-zA-Z_][a-zA-Z0-9_]* {
    YY_LVALP->ident = strdup(yytext);
    return EVAL_IDENT;
}

/* numbers */
[0-9]+\.[0-9]+([eE][+-]?[0-9]+)?  if (parse_float(&YY_LVALP->scalar, yytext)) return EVAL_FLOAT;
0x[0-9a-fA-F]+                    if (parse_integer(&YY_LVALP->scalar, yytext+2,16,0)) return EVAL_INT;
[0-9]+                            if (parse_integer(&YY_LVALP->scalar, yytext,10,0)) return EVAL_INT;

\n              return '\n';
.               return *yytext;

%%
