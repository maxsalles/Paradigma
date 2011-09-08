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
    PTK_ERROR_HEXADECIMAL_CONSTANT  = 11,
    PTK_ERROR_BINARY_CONSTANT       = 12,
    PTK_ERROR_OCTAL_CONSTANT        = 13,
    PTK_ERROR_FLOAT_CONSTANT        = 14,
    PTK_ERROR_DECIMAL_CONSTANT      = 15,
    PTK_ERROR_STRING_CONSTANT       = 16,
    PTK_ERROR_DIRECTIVE             = 17,

    PTK_BIN_INT_VAL                 = 50,
    PTK_OCTAL_INT_VAL               = 51,
    PTK_DECIMAL_INT_VAL             = 52,
    PTK_HEXADECIMAL_INT_VAL         = 53,
    PTK_FLOAT_VAL                   = 54,
    PTK_STRING_VAL                  = 55,

    PTK_OP_CLN                      = 140, /* :   */
    PTK_OP_ASG                      = 141, /* :=  */
    PTK_OP_IDT                      = 142, /* === */
    PTK_OP_NDT                      = 143, /* !== */
    PTK_OP_DOT                      = 144, /* .   */
    PTK_OP_ARR                      = 145, /* ->  */
    PTK_OP_ECP                      = 146, /* ::  */

    PTK_OP_ADD                      = 150, /* +   */
    PTK_OP_SUB                      = 151, /* -   */
    PTK_OP_MUL                      = 152, /* *   */
    PTK_OP_DIV                      = 153, /* /   */
    PTK_OP_MOD                      = 154, /* %   */
    PTK_OP_INC                      = 155, /* ++  */
    PTK_OP_DEC                      = 156, /* --  */

    PTK_OP_OR                       = 157, /* |   */
    PTK_OP_AND                      = 158, /* &   */
    PTK_OP_NOT                      = 159, /* !   */
    PTK_OP_SML                      = 160, /* <   */
    PTK_OP_BIG                      = 161, /* >   */
    PTK_OP_SEQ                      = 162, /* <=  */
    PTK_OP_BEG                      = 163, /* >=  */
    PTK_OP_EQL                      = 164, /* ==  */
    PTK_OP_DIF                      = 165, /* !=  */

    PTK_OP_TO                       = 166, /* ..  */

    PTK_OP_CPY                      = 167, /* =   */
    PTK_OP_CAD                      = 168, /* +=  */
    PTK_OP_CMN                      = 169, /* -=  */
    PTK_OP_CML                      = 170, /* *=  */
    PTK_OP_CDV                      = 171, /* /=  */
    PTK_OP_CMD                      = 172, /* %=  */

    PTK_COMMA                       = 180, /* ,   */
    PTK_SEMICOLON                   = 181, /* ;   */
    PTK_DOLLAR                      = 182, /* $   */

    PTK_LEFT_PARENTHESIS            = 190, /* (   */
    PTK_RIGHT_PARENTHESIS           = 191, /* )   */
    PTK_LEFT_BRACE                  = 192, /* [   */
    PTK_RIGHT_BRACE                 = 193, /* ]   */
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

