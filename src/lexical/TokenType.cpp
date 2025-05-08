#include "Pascal--/lexical/TokenType.hpp"

std::string tt2str(enum TokenType type) {
    switch (type) {
        // Specials
        case TT_EOF:               return "EOF";

        // Arithmetic operators
        case TT_ADD:               return "ADD";
        case TT_SUB:               return "SUB";
        case TT_MUL:               return "MUL";
        case TT_DIV:               return "DIV";
        case TT_MOD:               return "MOD";
        case TT_FLOORDIV:          return "FLOORDIV";

        // Logical, relational operators and assignments
        case TT_OR:                return "OR";
        case TT_AND:               return "AND";
        case TT_NOT:               return "NOT";
        case TT_EQL:               return "EQL";
        case TT_NEQ:               return "NEQ";
        case TT_GTR:               return "GTR";
        case TT_GEQ:               return "GEQ";
        case TT_LSS:               return "LSS";
        case TT_LEQ:               return "LEQ";
        case TT_ASSIGN:            return "ASSIGN";

        // Symbols
        case TT_SEMICOLON:         return "SEMICOLON";
        case TT_COMMA:             return "COMMA";
        case TT_PERIOD:            return "PERIOD";
        case TT_COLON:             return "COLON";
        case TT_LPAREN:            return "LPAREN";
        case TT_RPAREN:            return "RPAREN";
        case TT_QUOTES:            return "QUOTES";

        // Keywords
        case TT_PROGRAMSYM:        return "PROGRAMSYM";
        case TT_VARSYM:            return "VARSYM";
        case TT_TYPE_INT:          return "TYPE_INT";
        case TT_TYPE_REAL:         return "TYPE_REAL";
        case TT_TYPE_STR:          return "TYPE_STR";
        case TT_BEGINSYM:          return "BEGINSYM";
        case TT_ENDSYM:            return "ENDSYM";
        case TT_FORSYM:            return "FORSYM";
        case TT_TOSYM:             return "TOSYM";
        case TT_WHILESYM:          return "WHILESYM";
        case TT_DOSYM:             return "DOSYM";
        case TT_BREAKSYM:          return "BREAKSYM";
        case TT_CONTINUESYM:       return "CONTINUESYM";
        case TT_IFSYM:             return "IFSYM";
        case TT_ELSESYM:           return "ELSESYM";
        case TT_THENSYM:           return "THENSYM";
        case TT_WRITESYM:          return "WRITESYM";
        case TT_WRITELNSYM:        return "WRITELNSYM";
        case TT_READSYM:           return "READSYM";
        case TT_READLNSYM:         return "READLNSYM";

        // Others
        case TT_IDENT:             return "IDENT";
        case TT_LITERAL_OCT:       return "LITERAL_OCT";
        case TT_LITERAL_DEC:       return "LITERAL_DEC";
        case TT_LITERAL_HEX:       return "LITERAL_HEX";
        case TT_LITERAL_REAL:      return "LITERAL_REAL";
        case TT_LITERAL_STR:       return "LITERAL_STR";

        default: throw std::string("invalid token type");
    }
}
