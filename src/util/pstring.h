/*
** Prefix: pst
** $ Herond Robaina Salles
*/

/* ========================================================================== */

#ifndef _PRD_PST_LIB
#define _PRD_PST_LIB

#define PST_NOT_PRESENT (-1)

/* ========================================================================== */

char* pstCopy (const char* string);

int pstGetLocation (const char* string, const char** array);

char* pstCharToString (char character);

char* pstAddChar (char* string, unsigned position, char character);

char* pstCharAppend (char** string, char character);

/* ========================================================================== */

#endif

