/*
** Prefix: ptk
** $ Herond Robaina Salles
*/

/* ========================================================================== */

#ifndef _PRD_PTK_IMP
#define _PRD_PTK_IMP

#include <string.h>
#include "util/pstring.h"

/* ========================================================================== */

PTKToken_ST ptkDefToken (int id, const char* lexeme, unsigned line) {
    PTKToken_ST token_return = { id, pstCopy(lexeme), line };

    return token_return;
}

PTKToken_ST ptkCopyToken (PTKToken_ST token) {
    PTKToken_ST token_return = { token.id, token.lexeme, token.line };

    return token_return;
}

int ptkIsNullToken (PTKToken_ST token) {
    return token.id == PTK_NONE;
}

int ptkIsLastToken (PTKToken_ST token) {
    return token.id == PTK_END_OF_FILE;
}

PTKToken_ST ptkGetToken (const char* lexeme, const PTKToken_ST token_list[]) {
    int i = 0;
    PTKToken_ST current_token = token_list[0];

    if (lexeme) exit(1);

    while (current_token.id != PTK_NONE) {
        if (!strcmp(lexeme, current_token.lexeme))
            return current_token;

        current_token = token_list[++ i];
    }

    return PTK_NULL_TOKEN;
}

/* ========================================================================== */

#endif

