#include "Pascal--/lexical/TokenType.hpp"

std::string tt2str(enum TokenType type) {
    switch (type) {
        // Specials
        case TT_EOF:              return "EOF";

        // Arithmetic operators
        case TT_ADD:              return "ADD";
        case TT_SUB:              return "SUB";
        case TT_MUL:              return "MUL";
        case TT_DIV:              return "DIV";
        case TT_MOD:              return "MOD";
        case TT_FLOORDIV:         return "FLOORDIV";

        // Logical, relational operators and assignments
        case TT_OR:               return "OR";
        case TT_AND:              return "AND";
        case TT_NOT:              return "NOT";
        case TT_EQL:              return "EQL";
        case TT_NEQ:              return "NEQ";
        case TT_GTR:              return "GTR";
        case TT_GEQ:              return "GEQ";
        case TT_LSS:              return "LSS";
        case TT_LEQ:              return "LEQ";
        case TT_ASSIGN:           return "ASSIGN";

        // Symbols
        case TT_SEMICOLON:        return "SEMICOLON";
        case TT_COMMA:            return "COMMA";
        case TT_PERIOD:           return "PERIOD";
        case TT_COLON:            return "COLON";
        case TT_LPAREN:           return "LPAREN";
        case TT_RPAREN:           return "RPAREN";
        case TT_QUOTES:           return "QUOTES";

        // Keywords
        case TT_PROGRAM:          return "PROGRAM";
        case TT_VAR:              return "VAR";
        case TT_TYPE_INT:         return "TYPE_INT";
        case TT_TYPE_REAL:        return "TYPE_REAL";
        case TT_TYPE_STR:         return "TYPE_STR";
        case TT_BEGIN:            return "BEGIN";
        case TT_END:              return "END";
        case TT_FOR:              return "FOR";
        case TT_TO:               return "TO";
        case TT_WHILE:            return "WHILE";
        case TT_DO:               return "DO";
        case TT_BREAK:            return "BREAK";
        case TT_CONTINUE:         return "CONTINUE";
        case TT_IF:               return "IF";
        case TT_ELSE:             return "ELSE";
        case TT_THEN:             return "THEN";
        case TT_WRITE:            return "WRITE";
        case TT_WRITELN:          return "WRITELN";
        case TT_READ:             return "READ";
        case TT_READLN:           return "READLN";

        // Others
        case TT_IDENT:            return "IDENT";
        case TT_LITERAL_OCT:      return "LITERAL_OCT";
        case TT_LITERAL_DEC:      return "LITERAL_DEC";
        case TT_LITERAL_HEX:      return "LITERAL_HEX";
        case TT_LITERAL_REAL:     return "LITERAL_REAL";
        case TT_LITERAL_STR:      return "LITERAL_STR";

        default:                  return "INVALID_TOKEN";
    }
}
