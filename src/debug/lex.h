#ifndef __LEX_H__
#define __LEX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "debug/debugparse.h"   /* for YYSTYPE */

/* Flex buffer helpers */
void *lex_current_buffer(void);
int   lex_current_buffer_pos(void);
void  lex_switch_buffer(void *buffer);
void  lex_delete_buffer(void *buffer);
void *lex_scan_string_buffer(const char *str);

/* Reentrant lexer */
int yylex(YYSTYPE *lvalp);

#ifdef __cplusplus
}
#endif

#endif /* __LEX_H__ */
