#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include "pascal--/lexical/TokenType.hpp"

struct Lexeme {
    std::string token;
    int line;
    int column;
    enum TokenType type;

    Lexeme();
    Lexeme(const std::string& token, int line, int column, enum TokenType type);

    virtual ~Lexeme();

    std::string str() const;
};

#endif // LEXEME_HPP
