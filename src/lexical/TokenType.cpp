#include "Pascal--/lexical/TokenType.hpp"

std::string tt2str(enum TokenType type) {
    switch (type) {
        // Specials
        case TT_EOF:              return "eof";

        // Arithmetic operators
        case TT_ADD:              return "add";
        case TT_SUB:              return "sub";
        case TT_MUL:              return "mul";
        case TT_DIV:              return "div";
        case TT_MOD:              return "mod";
        case TT_FLOORDIV:         return "floordiv";

        // Logical, relational operators and assignments
        case TT_OR:               return "or";
        case TT_AND:              return "and";
        case TT_NOT:              return "not";
        case TT_EQL:              return "eql";
        case TT_NEQ:              return "neq";
        case TT_GTR:              return "gtr";
        case TT_GEQ:              return "geq";
        case TT_LSS:              return "lss";
        case TT_LEQ:              return "leq";
        case TT_ASSIGN:           return "assign";

        // Symbols
        case TT_SEMICOLON:        return "semicolon";
        case TT_COMMA:            return "comma";
        case TT_PERIOD:           return "period";
        case TT_COLON:            return "colon";
        case TT_LPAREN:           return "lparen";
        case TT_RPAREN:           return "rparen";
        case TT_QUOTE:            return "quote";

        // Keywords
        case TT_PROGRAM:          return "program";
        case TT_VAR:              return "var";
        case TT_TYPE_INT:         return "type int";
        case TT_TYPE_REAL:        return "type real";
        case TT_TYPE_STR:         return "type string";
        case TT_BEGIN:            return "begin";
        case TT_END:              return "end";
        case TT_FOR:              return "for";
        case TT_TO:               return "to";
        case TT_WHILE:            return "while";
        case TT_DO:               return "do";
        case TT_BREAK:            return "break";
        case TT_CONTINUE:         return "continue";
        case TT_IF:               return "if";
        case TT_ELSE:             return "else";
        case TT_THEN:             return "then";
        case TT_WRITE:            return "write";
        case TT_WRITELN:          return "writeln";
        case TT_READ:             return "read";
        case TT_READLN:           return "readln";

        // Others
        case TT_IDENT:            return "ident";
        case TT_LITERAL_OCT:      return "literal octal";
        case TT_LITERAL_DEC:      return "literal decimal";
        case TT_LITERAL_HEX:      return "literal hexadecimal";
        case TT_LITERAL_REAL:     return "literal real";
        case TT_LITERAL_STR:      return "literal string";

        default:                  return "invalid token";
    }
}
