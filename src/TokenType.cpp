#include "TokenType.hpp"

std::string tt2str(enum TokenType type) {
    switch (type) {
        // Specials
        case TT_UNEXPECTED_EOF:    return "UNEXPECTED_EOF";
        case TT_INVALID_TOKEN:     return "INVALID_TOKEN";
        case TT_END_OF_FILE:       return "END_OF_FILE";

        // Arithmetic operators
        case TT_ADD:               return "ADD";
        case TT_SUB:               return "SUB";
        case TT_MUL:               return "MUL";
        case TT_DIV:               return "DIV";
        case TT_MOD:               return "MOD";
        case TT_DIVINT:            return "DIVINT";

        // Logical, relational operators and assignments
        case TT_OR:                return "OR";
        case TT_AND:               return "AND";
        case TT_NOT:               return "NOT";
        case TT_EQUAL:             return "EQUAL";
        case TT_DIFFERENCE:        return "DIFFERENCE";
        case TT_GREATER:           return "GREATER";
        case TT_GREATER_EQUAL:     return "GREATER_EQUAL";
        case TT_LOWER:             return "LOWER";
        case TT_LOWER_EQUAL:       return "LOWER_EQUAL";
        case TT_ASSIGN:            return "ASSIGN";

        // Symbols
        case TT_SEMICOLON:         return "SEMICOLON";
        case TT_COMMA:             return "COMMA";
        case TT_PERIOD:            return "PERIOD";
        case TT_COLON:             return "COLON";
        case TT_OPEN_PARENTHESES:  return "OPEN_PARENTHESES";
        case TT_CLOSE_PARENTHESES: return "CLOSE_PARENTHESES";
        case TT_QUOTES:            return "QUOTES";

        // Keywords
        case TT_PROGRAM:           return "PROGRAM";
        case TT_VAR:               return "VAR";
        case TT_TYPE_INTEGER:      return "TYPE_INTEGER";
        case TT_TYPE_REAL:         return "TYPE_REAL";
        case TT_TYPE_STRING:       return "TYPE_STRING";
        case TT_BEGIN:             return "BEGIN";
        case TT_END:               return "END";
        case TT_FOR:               return "FOR";
        case TT_TO:                return "TO";
        case TT_WHILE:             return "WHILE";
        case TT_DO:                return "DO";
        case TT_BREAK:             return "BREAK";
        case TT_CONTINUE:          return "CONTINUE";
        case TT_IF:                return "IF";
        case TT_ELSE:              return "ELSE";
        case TT_THEN:              return "THEN";
        case TT_WRITE:             return "WRITE";
        case TT_WRITELN:           return "WRITELN";
        case TT_READ:              return "READ";
        case TT_READLN:            return "READLN";

        // Others
        case TT_VAR_NAME:          return "VAR_NAME";
        case TT_LITERAL_OCTAL:     return "LITERAL_OCTAL";
        case TT_LITERAL_DECIMAL:   return "LITERAL_DECIMAL";
        case TT_LITERAL_HEX:       return "LITERAL_HEX";
        case TT_LITERAL_REAL:      return "LITERAL_REAL";
        case TT_LITERAL_STRING:    return "LITERAL_STRING";

        default: throw std::string("invalid token type");
    }
}
