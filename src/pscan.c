/*
** Prefix: psc
** $ Herond Robaina Salles
*/

/* ========================================================================== */

#ifndef _PRD_PSC_IMP
#define _PRD_PSC_IMP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pstring.h"
#include "ptoken.h"

/* ========================================================================== */

static int isHexadecimalDigit (char character) {
    return isdigit(character)                     ||
           (character >= 'a' && character <= 'f') ||
           (character >= 'A' && character <= 'F');
}

static int isOctalDigit (char character) {
    return character >= '0' && character < '8';
}

static int isCharacterForId (char character) {
    return isalnum(character) || character == '_';
}

/* ========================================================================== */

static PTKToken_ST isIdentifier (FILE* source_code, unsigned line) {
    if (source_code && reserved_words) {
        char word_return[51] = "\0";
        char character = getc(source_code);
        int i = 0;

        while (isCharacterForId(character) && i < 50) {
            pstAddChar(word_return, i++, character);
            character = getc(source_code);
        }

        fseek(source_code, -1, SEEK_CUR);

        return ptkDefToken(PTK_ID, word_return, line);
    } else exit(1); /* trocar por um erro apropriado */
}

/* ========================================================================== */

static PTKToken_ST isDirective (FILE* source_code, unsigned line) {
    if (source_code && reserved_words) {
        char word_return[51] = "@";
        char character = getc(source_code);
        int i = 0;
        PTKToken_ST token_return = PTK_NULL_TOKEN;

        while (isCharacterForId(character) && i < 50) {
            pstAddChar(word_return, i++, character);
            character = getc(source_code);
        }

        fseek(source_code, -1, SEEK_CUR);
        token_return = ptkGetToken(word_return, PTK_DIRECTIVES);
        token_return.line = line;

        if ptkIsNullToken(token_return)
            return ptkDefToken(PTK_ERROR_DIRECTIVE, word_return, line);

        return token_return;
    } else exit(1); /* trocar por um erro apropriado */
}

/* ========================================================================== */

static PTKToken_ST isStringConstant (FILE* source_code, unsigned line) {
    if (source_code) {
        char* string_return = pstCopy("\0");
        char character = getc(source_code);
        int flag_aux = 0;

        for(;;) {
            if (character == '\\' ) flag_aux = 1;
            else if (character == '/'  ) flag_aux = 2;
            else if (character == '\"' && flag_aux != 2   )
                return ptkDefToken(PTK_STRING_VAL, string_return, line);
            else if
                ((character == '\n' || character == EOF ) && flag_aux != 1) {
                fseek(source_code, -1, SEEK_CUR);

                return
                    ptkDefToken(PTK_ERROR_STRING_CONSTANT, string_return, line);
            }
            pstCharApend(&string_return, character);

            character = getc(source_code);
        }
    } else exit(1);
}

/* ========================================================================== */

static PTKToken_ST isDecimalOrFloatConstant (FILE* source_code, unsigned line) {
    if (source_code) {
        char constant_return[51] = "\0";
        char character = getc(source_code);
        int i = 0, flag_float = 0;

        /* pega a parte inteira */
        while (isdigit(character) && i < 50) {
            pstAddChar(constant_return, i++, character);
            character = getc(source_code);
        }

        /* pega o ponto */
        if (character == '.') {
            pstAddChar(constant_return, i++, character);

            /* emite um erro caso não exista a parte fracionária */
            character = getc(source_code);
            if (!isdigit(character)) {
                while (isCharacterForId(character)) {
                    pstAddChar(constant_return, i++, character);
                    character = getc(source_code);
                }

                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(
                    PTK_ERROR_DECIMAL_CONSTANT, constant_return, line
                );
            }

            /* pega a parte fracionária */
            while (isdigit(character) && i < 50) {
                pstAddChar(constant_return, i++, character);
                character = getc(source_code);
            }

            /* pega o 'e' ou o 'E' */
            if (character == 'e' || character == 'E') {
                pstAddChar(constant_return, i++, character);

                /* paga o sinal '-' ou '+' */
                character = getc(source_code);
                if (character == '-' || character == '+') {
                    pstAddChar(constant_return, i++, character);
                    character = getc(source_code);
                }

                /* emite um erro caso não haja expoente */
                if (!isdigit(character)) {
                    while (isCharacterForId(character)) {
                        pstAddChar(constant_return, i++, character);
                        character = getc(source_code);
                    }

                    fseek(source_code, -1, SEEK_CUR);

                    return ptkDefToken(
                        PTK_ERROR_DECIMAL_CONSTANT, constant_return, line
                    );
                }

                /* pega o expoente */
                while (isdigit(character)) {
                    pstAddChar(constant_return, i++, character);
                    character = getc(source_code);
                }
            }

            flag_float = 1;
        }

        /* emite erro caso não haja um delimitador */
        if (isCharacterForId(character)) {
            while (isCharacterForId(character)) {
                pstAddChar(constant_return, i++, character);
                character = getc(source_code);
            }

            fseek(source_code, -1, SEEK_CUR);

            return
                ptkDefToken(PTK_ERROR_DECIMAL_CONSTANT, constant_return, line);
        } else {
            fseek(source_code, -1, SEEK_CUR);

            return flag_float ?
                ptkDefToken(PTK_FLOAT_VAL, constant_return, line) :
                ptkDefToken(PTK_DECIMAL_INT_VAL, constant_return, line);
        }
    } else exit(1); /* trocar por um erro apropriado */
}

static PTKToken_ST isOctalConstant (FILE* source_code, unsigned line) {
    if (source_code) {
        char constant_return[51] = "\0";
        char character = getc(source_code);
        int flag_error = 0, i = 0;

        while ((isalnum(character) || character == '_') && i < 50) {
            pstAddChar(constant_return, i++, character);

            if (!flag_error && !isOctalDigit(character)) flag_error = 1;

            character = getc(source_code);
        }

        fseek(source_code, -1, SEEK_CUR);

        if (flag_error)
             return
                ptkDefToken(PTK_ERROR_OCTAL_CONSTANT, constant_return, line);
        else return ptkDefToken(PTK_OCTAL_INT_VAL, constant_return, line);
    } else exit(1); /* trocar por um erro apropriado */
}

static PTKToken_ST isHexadecimalConstant (FILE* source_code, unsigned line) {
    if (source_code) {
        char constant_return[51] = "\0";
        char character = getc(source_code);
        int flag_error = 0, i = 0;

        while ((isalnum(character) || character == '_') && i < 50) {
            pstAddChar(constant_return, i++, character);

            if (!flag_error && !isHexadecimalDigit(character)) flag_error = 1;
            character = getc(source_code);
        }

        fseek(source_code, -1, SEEK_CUR);
        if (flag_error)
            return ptkDefToken(
                PTK_ERROR_HEXADECIMAL_CONSTANT, constant_return, line
            );
        else return ptkDefToken(PTK_HEXADECIMAL_INT_VAL, constant_return, line);
    } exit(1); /* trocar por um erro apropriado */
}

static PTKToken_ST isNumericConstant (FILE* source_code, unsigned line) {
    if (source_code) {
        switch (getc(source_code)) {
        case '0':
            switch (getc(source_code)) {
            case 'x': case 'X':

                return isHexadecimalConstant(source_code, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return isOctalConstant(source_code, line);
            }
        default :
            fseek(source_code, -1, SEEK_CUR);

            return isDecimalOrFloatConstant(source_code, line);
        }
    } else exit(1); /* trocar por um erro apropriado */
}

/* ========================================================================== */

PTKToken_ST pscIdentifyComment (FILE* source_code, unsigned line) {
    if (source_code) {
        char* comment_return = pstCopy("/*");
        char character = getc(source_code);
        int flag_end = 0;

        while(character != EOF) {
            if (character == '/' && flag_end) {
                pstCharApend(&comment_return, character);

                return ptkDefToken(PTK_COMMENT, comment_return, line);
            }

            flag_end  = character == '*';
            pstCharApend(&comment_return, character);
            character = getc(source_code);
        }

        return ptkDefToken(PTK_ERROR_COMMENT, comment_return, line);
    } else exit(1); /* trocar por um erro apropriado */
}

/* ========================================================================== */

PTKToken_ST pscGetToken (FILE* source_code) {
    static unsigned line = 1;

    if (source_code) {
        char character = getc(source_code);

        switch (character) {
        case EOF: /* eof */
            return ptkDefToken(PTK_END_OF_FILE, NULL, line);

        case '!': /* !, !=, !== */
            switch (getc(source_code)) {
            case '=':
                switch (getc(source_code)) {
                case '=': return ptkDefToken(PTK_OP_NDT, NULL, line);
                default :
                    fseek(source_code, -1, SEEK_CUR);

                    return ptkDefToken(PTK_OP_DIF, NULL, line);
                }
            default :
                fseek(source_code, -1,SEEK_CUR);

                return ptkDefToken(PTK_OP_NOT, NULL, line);
            }

        case '+': /* +, ++, += */
            switch (getc(source_code)) {
            case '+': return ptkDefToken(PTK_OP_INC, NULL, line);
            case '=': return ptkDefToken(PTK_OP_CAD, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_ADD, NULL, line);
            }

        case '-': /* -, --, -= */
            switch (getc(source_code)) {
            case '-': return ptkDefToken(PTK_OP_DEC, NULL, line);
            case '=': return ptkDefToken(PTK_OP_CMN, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_SUB, NULL, line);
            }

        case '*': /* *, *= */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_CML, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_MUL, NULL, line);
            }

        case '/': /* /, /=, comment */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_CDV, NULL, line);
            case '*': return pscIdentifyComment(source_code, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_DIV, NULL, line);
            }

        case '%': /* %, %= */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_CMD, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_MOD, NULL, line);
            }

        case '.': /* ., ..  */
            switch (getc(source_code)) {
            case '.': return ptkDefToken(PTK_OP_TO, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_DOT, NULL, line);
            }

        case '<': /* <, <=, << */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_SEQ, NULL, line);
            case '<': return ptkDefToken(PTK_OP_DLF, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_SML, NULL, line);
            }

        case '>': /* >, >=, >> */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_BEG, NULL, line);
            case '>': return ptkDefToken(PTK_OP_DRG, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_BIG, NULL, line);
            }

        case '=': /* =, ==, === */
            switch (getc(source_code)) {
            case '=':
                switch (getc(source_code)) {
                case '=': return ptkDefToken(PTK_OP_IDT, NULL, line);
                default :
                    fseek(source_code, -1, SEEK_CUR);

                    return ptkDefToken(PTK_OP_EQL, NULL, line);
                }
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_CPY, NULL, line);
            }

        case '&': /* & */
            return ptkDefToken(PTK_OP_AND, NULL, line);

        case '|': /* | */
            return ptkDefToken(PTK_OP_OR, NULL, line);

        case ':': /* :, := */
            switch (getc(source_code)) {
            case '=': return ptkDefToken(PTK_OP_ASG, NULL, line);
            default :
                fseek(source_code, -1, SEEK_CUR);

                return ptkDefToken(PTK_OP_CLN, NULL, line);
            }

        case '$': /* $ */
            return ptkDefToken(PTK_DOLLAR, NULL, line);

        case ';': /* ; */
            return ptkDefToken(PTK_SEMICOLON, NULL, line);

        case '(': /* ( */
            return ptkDefToken(PTK_LEFT_PARENTHESIS, NULL, line);

        case ')': /* ) */
            return ptkDefToken(PTK_RIGHT_PARENTHESIS, NULL, line);

        case '{': /* { */
            return ptkDefToken(PTK_LEFT_BRACE, NULL, line);

        case '}': /* } */
            return ptkDefToken(PTK_RIGHT_BRACE, NULL, line);

        case '\"': return isStringConstant(source_code, line);

        case '\n': /* none */
            line ++;

            return PTK_NULL_TOKEN;

        case '@': return isDirective(source_code, line);

        default  :
            if (isdigit(character)) {
                fseek(source_code, -1, SEEK_CUR);

                return isNumericConstant(source_code, line);
            } else if (isalpha(character) || character == '_') {
                fseek(source_code, -1, SEEK_CUR);

                return isIdentifier(source_code, line);
            } else return PTK_NULL_TOKEN;
        }

    } exit(1); /* trocar por um erro apropriado */
}

PTKToken_ST pscGetTokenIgnoring (int token, FILE* source_code) {
    if (source_code) {
        PTKToken_ST current_token = pscGetToken(source_code);

        while (current_token.id == token && current_token.id != PTK_END_OF_FILE)
            current_token = pscGetToken(source_code);

        return current_token;
    } exit(1); /* trocar por um erro apropriado */
}

/* ========================================================================== */

#endif

