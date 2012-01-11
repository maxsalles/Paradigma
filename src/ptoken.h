/*
** Prefix: ptk
** $ Herond Robaina Salles
*/

/* ========================================================================== */

#ifndef _PRD_PTK_LIB
#define _PRD_PTK_LIB

#define PTK_NULL_TOKEN 		(ptkDefToken(PTK_NONE, NULL, 0))
#define PTK_RESERVED_WORDS	(ptkReservedWords())

/* ========================================================================== */

enum PTKTokenId_EN {
    PTK_END_OF_FILE                 = 0,
    PTK_NONE                        = 1,

    PTK_ERROR_COMMENT               = 10,
    PTK_ERROR_OCTAL_CONSTANT        = 11,
    PTK_ERROR_DECIMAL_CONSTANT      = 12,
    PTK_ERROR_HEXADECIMAL_CONSTANT  = 13,
    PTK_ERROR_FLOAT_CONSTANT        = 14,
    PTK_ERROR_STRING_CONSTANT       = 15,
    PTK_ERROR_DIRECTIVE             = 16,

    PTK_OCTAL_INT_VAL               = 50,
    PTK_DECIMAL_INT_VAL             = 51,
    PTK_HEXADECIMAL_INT_VAL         = 52,
    PTK_FLOAT_VAL                   = 53,
    PTK_STRING_VAL                  = 54,

    PTK_OP_CLN                      = 140, /* :   */
    PTK_OP_DOT                      = 141, /* .   */

    PTK_OP_ASG                      = 142, /* :=  */
    PTK_OP_CPY                      = 143, /* =   */
    PTK_OP_CAD                      = 144, /* +=  */
    PTK_OP_CMN                      = 145, /* -=  */
    PTK_OP_CML                      = 146, /* *=  */
    PTK_OP_CDV                      = 147, /* /=  */
    PTK_OP_CMD                      = 148, /* %=  */

    PTK_OP_ADD                      = 149, /* +   */
    PTK_OP_SUB                      = 150, /* -   */
    PTK_OP_MUL                      = 151, /* *   */
    PTK_OP_DIV                      = 152, /* /   */
    PTK_OP_MOD                      = 153, /* %   */
    PTK_OP_INC                      = 154, /* ++  */
    PTK_OP_DEC                      = 155, /* --  */
    PTK_OP_DLF                      = 156, /* <<  */
    PTK_OP_DRG                      = 157, /* >>  */

    PTK_OP_OR                       = 158, /* |   */
    PTK_OP_AND                      = 159, /* &   */
    PTK_OP_NOT                      = 160, /* !   */
    PTK_OP_SML                      = 161, /* <   */
    PTK_OP_BIG                      = 162, /* >   */
    PTK_OP_SEQ                      = 163, /* <=  */
    PTK_OP_BEG                      = 164, /* >=  */
    PTK_OP_EQL                      = 165, /* ==  */
    PTK_OP_DIF                      = 166, /* !=  */
    PTK_OP_IDT                      = 167, /* === */
    PTK_OP_NDT                      = 168, /* !== */

    PTK_OP_TO                       = 169, /* ..  */

    PTK_SEMICOLON                   = 181, /* ;   */
    PTK_DOLLAR                      = 182, /* $   */

    PTK_LEFT_PARENTHESIS            = 190, /* (   */
    PTK_RIGHT_PARENTHESIS           = 191, /* )   */
    PTK_LEFT_BRACKET                = 197, /* {   */
    PTK_RIGHT_BRACKET               = 198, /* }   */

    PTK_SELF                        = 200,
    PTK_CONST                       = 201,
    PTK_OPERATOR                    = 202,
    PTK_USES                        = 203,
    PTK_PUBLIC                      = 204,
    PTK_PRIVATE                     = 205,
    PTK_MAIN                        = 206,

    PTK_ID                          = 220,
    PTK_COMMENT                     = 221
};

const PTKToken_ST PTK_DIRECTIVES[] = {
	{ PTK_SELF,     "@self",     0 },
	{ PTK_CONST,    "@const",    0 },
	{ PTK_OPERATOR, "@operator", 0 },
	{ PTK_USES,     "@uses",     0 },
	{ PTK_PUBLIC,   "@public",   0 },
	{ PTK_PRIVATE,  "@private",  0 },
	{ PTK_MAIN,     "@main",     0 },

	{ PTK_NONE,      NULL,       0 }
};

typedef struct {
    int id;
    char* lexeme;
    unsigned line;
} PTKToken_ST;

/* ========================================================================== */

PTKToken_ST ptkDefToken (int id, const char* lexeme, unsigned line);

PTKToken_ST ptkCopyToken (PTKToken_ST token);

int ptkIsNullToken (PTKToken_ST token);

int ptkIsLastToken (PTKToken_ST token);

PTKToken_ST ptkGetToken (const char* lexeme, const PTKToken_ST token_list[]);

/* ========================================================================== */

#endif

