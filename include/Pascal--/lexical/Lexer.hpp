#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "Pascal--/lexical/Lexeme.hpp"
#include "Pascal--/lexical/SymbolTable.hpp"
#include "Pascal--/util/File.hpp"

class Lexer {
public:
    Lexer();
    virtual ~Lexer();

    const std::vector<Lexeme>& scan_file(const char* filename);

    const std::vector<Lexeme>& lexemes() { return m_lexemes; }

private:
    Lexeme make_lexeme();
    std::string lexical_error(const std::string& message, Lexeme& lexeme);

    File m_file;
    std::vector<Lexeme> m_lexemes;
};

#endif // LEXER_HPP
