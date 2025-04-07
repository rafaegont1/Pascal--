#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include "TokenType.hpp"

struct Lexeme {
	std::string token;
	enum TokenType type;

	Lexeme();
	virtual ~Lexeme();

	std::string str() const;
};

#endif // LEXEME_HPP
