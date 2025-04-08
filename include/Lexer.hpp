#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstdio>
#include "Lexeme.hpp"
#include "SymbolTable.hpp"

class Lexer {
public:
	Lexer(const char* filename);
	virtual ~Lexer();

	Lexeme nextToken();
	int line() const { return m_line; }
	int column() const { return m_column; }

private:
	void newLine();

	int m_line;
	int m_column;
	SymbolTable m_symbolTable;
	FILE* m_input;
};

#endif // LEXER_HPP
