#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include "TokenType.hpp"

struct Lexeme {
    std::string token;
    enum TokenType type;
    int line;
    int column;

    Lexeme();
    virtual ~Lexeme();

    void setPosition(int line, int column);
    std::string str() const;
};

#endif // LEXEME_HPP
