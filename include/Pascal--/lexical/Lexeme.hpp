#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include "Pascal--/lexical/TokenType.hpp"

struct Lexeme {
    std::string token;
    enum TokenType type;
    int line;
    int column;

    Lexeme();
    Lexeme(const std::string& token, enum TokenType type, int line, int column);

    virtual ~Lexeme();

    std::string str() const;
};

#endif // LEXEME_HPP
