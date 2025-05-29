#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <cstdint>
#include <string>
#include "Pascal--/lexical/TokenType.hpp"

struct Lexeme {
    std::string token;
    enum TokenType type;
    uint16_t line;
    uint16_t column;

    Lexeme();
    Lexeme(
        const std::string& token,
        enum TokenType type,
        uint16_t line, uint16_t column
    );

    virtual ~Lexeme();

    std::string str() const;
};

#endif // LEXEME_HPP
