#ifndef LEXEME_HPP
#define LEXEME_HPP

#include <string>
#include "pascal--/lexical/TokenType.hpp"

/**
 *  @struct Lexeme
 *  @brief Represents a lexical token with its metadata.
 */
struct Lexeme {
    std::string token;
    enum TokenType type;
    int line;
    int column;

    /**
     *  @brief Constructs an empty Lexeme with default values.
     */
    Lexeme();

    /**
     *  @brief Virtual destructor to allow for inheritance.
     */
    virtual ~Lexeme();

    /**
     *  @brief Generates a formatted string representation of the lexeme.
     *  @return String in format "[line:column] <type> 'token'"
     */
    std::string str() const;
};

#endif // LEXEME_HPP
