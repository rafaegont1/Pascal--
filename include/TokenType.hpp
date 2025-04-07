#ifndef TOKEN_TYPE_HPP
#define TOKEN_TYPE_HPP

#include <string>

enum TokenType {
	// Specials
	TT_UNEXPECTED_EOF = -2,
	TT_INVALID_TOKEN  = -1,
	TT_END_OF_FILE    =  0,

	// Arithmetic operators
	TT_ADD,                // +
	TT_SUB,                // -
	TT_MUL,                // *
	TT_DIV,                // /
	TT_MOD,                // mod
	TT_DIVINT,             // div

	// Logical, relational operators and assignments
	TT_OR,                 // or
	TT_AND,                // and
	TT_NOT,                // not
	TT_EQUAL,              // ==
	TT_DIFFERENCE,         // <>
	TT_GREATER,            // >
	TT_GREATER_EQUAL,      // >=
	TT_LOWER,              // <
	TT_LOWER_EQUAL,        // <=
	TT_ASSIGN,             // :=

	// Keywords
	TT_PROGRAM,            // program
	TT_VAR,                // var
	TT_TYPE_INTEGER,       // integer
	TT_TYPE_REAL,          // real
	TT_TYPE_STRING,        // string
	TT_BEGIN,              // begin
	TT_END,                // end
	TT_FOR,                // for
	TT_TO,                 // to
	TT_WHILE,              // while
	TT_DO,                 // do
	TT_BREAK,              // break
	TT_CONTINUE,           // continue
	TT_IF,                 // if
	TT_ELSE,               // else
	TT_THEN,               // then
	TT_WRITE,              // write
	TT_WRITELN,            // writeln
	TT_READ,               // read
	TT_READLN,             // read

	// Symbols
	TT_SEMICOLON,          // ;
	TT_COMMA,              // ,
	TT_PERIOD,             // .
	TT_COLON,              // :
	TT_OPEN_PARENTHESES,   // (
	TT_CLOSE_PARENTHESES,  // )
	TT_QUOTES,             // "

	// Others
	TT_VAR_NAME,
	TT_LITERAL_INTEGER,
	TT_LITERAL_REAL,
	TT_LITERAL_STRING,
};

std::string tt2str(enum TokenType type);

#endif // TOKEN_TYPE_HPP
