#ifndef LEXER_HPP
#define LEXER_HPP

#include <cstdio>
#include "pascal--/lexical/Lexeme.hpp"
#include "pascal--/lexical/SymbolTable.hpp"

/**
 *  @class Lexer
 *  @brief A lexical analyzer that processes input from a file and generates tokens.
 */
class Lexer {
public:
    /**
     *  @brief Constructs a Lexer object with the specified input file.
     *  @param filename The path to the input file to be tokenized.
     */
    Lexer(const char* filename);
    
    /**
     *  @brief Virtual destructor to allow for proper cleanup in derived classes.
     */
    virtual ~Lexer();

    /**
     *  @brief Retrieves the next token from the input stream.
     *  @return The next Lexeme (token) from the input.
     */
    Lexeme nextToken();
    
    /**
     *  @brief Gets the current line number in the input file.
     *  @return The line number where the lexer is currently positioned.
     */
    int line() const { return m_line; }
    
    /**
     *  @brief Gets the current column number in the input file.
     *  @return The column number where the lexer is currently positioned.
     */
    int column() const { return m_column; }

private:
    /**
     *  @brief Handles newline characters and updates position tracking.
     *  @param lexeme The current lexeme being built.
     */
    void newLine(Lexeme& lexeme);
    
    /**
     *  @brief Pushes a character back into the input stream.
     *  @param c The character to unget.
     */
    void ungetChar(int c);

    // Member variables
    int m_line;
    int m_column;
    FILE* m_input;
};

#endif // LEXER_HPP
