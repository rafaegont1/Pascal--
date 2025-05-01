#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <string>

enum TokenType {
    // Specials
    TT_INVALID        = -1,
    TT_EOF    =  0,

    // Identifier
    TT_IDENT,

    // Arithmetic operators
    TT_ADD,                // +
    TT_SUB,                // -
    TT_MUL,                // *
    TT_DIV,                // /
    TT_MOD,                // mod
    TT_FLOORDIV,           // div

    // Logical, relational operators and assignments
    TT_OR,                 // or
    TT_AND,                // and
    TT_NOT,                // not
    TT_EQL,              // = ==
    TT_NEQ,         // <>
    TT_GTR,            // >
    TT_GEQ,      // >=
    TT_LSS,              // <
    TT_LEQ,        // <=
    TT_ASSIGN,             // :=

    // Keywords
    TT_PROGRAMSYM,            // program
    TT_VARSYM,                // var
    TT_TYPE_INTEGER,       // integer
    TT_TYPE_REAL,          // real
    TT_TYPE_STRING,        // string
    TT_BEGINSYM,              // begin
    TT_ENDSYM,                // end
    TT_FORSYM,                // for
    TT_TOSYM,                 // to
    TT_WHILESYM,              // while
    TT_DOSYM,                 // do
    TT_BREAKSYM,              // break
    TT_CONTINUESYM,           // continue
    TT_IFSYM,                 // if
    TT_ELSESYM,               // else
    TT_THENSYM,               // then
    TT_WRITESYM,              // write
    TT_WRITELNSYM,            // writeln
    TT_READSYM,               // read
    TT_READLNSYM,             // readln

    // Symbols
    TT_SEMICOLON,          // ;
    TT_COMMA,              // ,
    TT_PERIOD,             // .
    TT_COLON,              // :
    TT_LPAREN,             // (
    TT_RPAREN,             // )
    TT_QUOTES,             // "

    // Literals
    TT_LITERAL_OCTAL,
    TT_LITERAL_DECIMAL,
    TT_LITERAL_HEX,
    TT_LITERAL_REAL,
    TT_LITERAL_STRING,
};

std::string tt2str(enum TokenType type);

#endif // TOKEN_TYPE_HPP
