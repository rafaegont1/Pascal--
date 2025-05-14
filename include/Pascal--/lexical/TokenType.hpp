#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <string>

enum TokenType {
    // Specials
    TT_INVALID = -1,
    TT_EOF = 0,

    // Identifier
    TT_IDENT,

    // Arithmetic operators
    TT_ADD,             // +
    TT_SUB,             // -
    TT_MUL,             // *
    TT_DIV,             // /
    TT_MOD,             // mod
    TT_FLOORDIV,        // div

    // Logical, relational operators and assignments
    TT_OR,              // or
    TT_AND,             // and
    TT_NOT,             // not
    TT_EQL,             // = ==
    TT_NEQ,             // <>
    TT_GTR,             // >
    TT_GEQ,             // >=
    TT_LSS,             // <
    TT_LEQ,             // <=
    TT_ASSIGN,          // :=

    // Keywords
    TT_PROGRAM,         // program
    TT_VAR,             // var
    TT_TYPE_INT,        // integer
    TT_TYPE_REAL,       // real
    TT_TYPE_STR,        // string
    TT_BEGIN,           // begin
    TT_END,             // end
    TT_FOR,             // for
    TT_TO,              // to
    TT_WHILE,           // while
    TT_DO,              // do
    TT_BREAK,           // break
    TT_CONTINUE,        // continue
    TT_IF,              // if
    TT_ELSE,            // else
    TT_THEN,            // then
    TT_WRITE,           // write
    TT_WRITELN,         // writeln
    TT_READ,            // read
    TT_READLN,          // readln

    // Symbols
    TT_SEMICOLON,       // ;
    TT_COMMA,           // ,
    TT_PERIOD,          // .
    TT_COLON,           // :
    TT_LPAREN,          // (
    TT_RPAREN,          // )
    TT_QUOTES,          // "

    // Literals
    TT_LITERAL_OCT,
    TT_LITERAL_DEC,
    TT_LITERAL_HEX,
    TT_LITERAL_REAL,
    TT_LITERAL_STR,

    TT_END_OF_ENUM
};

std::string tt2str(enum TokenType type);

#endif // TOKEN_TYPE_HPP
